#include "DoorControl.hpp"
#include "Rte_Door.h"
#include "Rte_Types.h"
#include "Rte_Sensor.h"
#include "cmsis_os2.h"
#include "uart.h"
#include "can_if.h"
#include "wdg_manager.h"

extern osMessageQueueId_t doorQueue;
DoorControl::DoorControl() { currentDoorState = 0; }
void DoorControl::Init() {}

void DoorControl::RunTask() {
    UART0_SendString("🚪 [Task Door] Ready. Managing Door State & Security...\r\n");
    
    bool is_door_locked = false;
    
    // Biến phục vụ FR-004 (Fade-out Đèn trần)
    bool is_fading = false;
    int fade_timer = 0;

    while(1) {
        WdgM_CheckpointReached(WDG_DOOR_TASK_ID);
        SystemEvent_t received_msg;

        // 1. EVENT-DRIVEN LÀM VIỆC VỚI NÚT BẤM
        if (osMessageQueueGet(doorQueue, &received_msg, NULL, 0) == osOK) {
            CanMessage_t tx_msg;
            tx_msg.dlc = 1;
            tx_msg.id = 0x101;

            if (received_msg == SYS_EVT_CRASH) {
                UART0_SendString("💥 [FR-002] CRASH RECEIVED! Auto-unlocking all doors for escape!\r\n");
                is_door_locked = false;
            }
            else if (received_msg == SYS_EVT_DOOR_OPENED) {
                Rte_Write_P_DoorLed_Status(LED_ON);
                is_door_locked = false;
                is_fading = false; // Nếu đang đếm lùi mà mở cửa lại thì hủy đếm
                
                tx_msg.data[0] = 1;
                CanIf_Transmit(&tx_msg);
                UART0_SendString("🚪 [Task Door] DOOR OPEN -> LED ON (Feedback Sent)\r\n");
            }
            else if (received_msg == SYS_EVT_DOOR_CLOSED) {
                // FR-004: Gửi mạng CAN báo đóng cửa ngay, nhưng ĐÈN CHƯA TẮT
                tx_msg.data[0] = 0;
                CanIf_Transmit(&tx_msg);
                
                UART0_SendString("🚪 [FR-004] Door Closed. Interior Light fading out in 5s...\r\n");
                is_fading = true;
                fade_timer = 100; // 100 chu kỳ * 50ms = 5000ms = 5 giây
            }
        }

        // 2. BỘ ĐẾM TIMER CHO FR-004
        if (is_fading) {
            fade_timer--;
            if (fade_timer <= 0) {
                Rte_Write_P_DoorLed_Status(LED_OFF); // Lúc này mới thực sự tắt đèn
                UART0_SendString("💡 [FR-004] Interior Light Faded Out Completely!\r\n");
                is_fading = false;
            }
        }

        // 3. FR-001 (AUTO DOOR LOCK)
        uint8_t current_speed = Rte_Read_VehicleSpeed();
        if (current_speed >= 20 && !is_door_locked) {
            UART0_SendString("🚨 [FR-001] Speed >= 20 km/h detected! AUTO-LOCKING ALL DOORS!\r\n");
            is_door_locked = true;
        }
        if (current_speed == 0 && is_door_locked) {
            UART0_SendString("🟢 [Task Door] Vehicle stopped (0 km/h). AUTO-UNLOCKING DOORS!\r\n");
            is_door_locked = false;
        }

        osDelay(50);
    }
}

DoorControl myDoor;
extern "C" void DoorControl_Init_Wrapper(void) { myDoor.Init(); }
extern "C" void DoorControl_Task_Wrapper(void *argument) { myDoor.RunTask(); }