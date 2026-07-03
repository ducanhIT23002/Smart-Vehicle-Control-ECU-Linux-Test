#include "DoorControl.hpp"
#include "Rte_Door.h"
#include "Rte_Types.h"
#include "cmsis_os2.h"
#include "uart.h"
#include "can_if.h"
#include "wdg_manager.h" // Khôi phục Watchdog của ông

extern osMessageQueueId_t doorQueue;

// ============================================================================
// CONSTRUCTOR
// ============================================================================
DoorControl::DoorControl() {
    currentDoorState = 0;
}

// ============================================================================
// PHƯƠNG THỨC: INIT
// ============================================================================
void DoorControl::Init() {
    // Nếu ông có logic khởi tạo cứng nào thì vứt vào đây
}

// ============================================================================
// PHƯƠNG THỨC: RUN TASK (Giữ nguyên 100% logic cũ của ông)
// ============================================================================
void DoorControl::RunTask() {
    UART0_SendString("[Task Door] Ready & Waiting...\r\n");

    while(1) {
        // Điểm danh với Watchdog (Chống treo)
        WdgM_CheckpointReached(WDG_DOOR_TASK_ID);
        
        SystemEvent_t received_msg;
        // Chờ 500ms thay vì osWaitForever để Watchdog không bị cắn
        if (osMessageQueueGet(doorQueue, &received_msg, NULL, 0) == osOK) {
            CanMessage_t tx_msg;
            tx_msg.dlc = 1;
            tx_msg.id = 0x101;

            if (received_msg == SYS_EVT_CRASH) {
                UART0_SendString("[Task Door] CRASH RECEIVED\r\n");
                while(1) {} // Giả lập treo Task (Crash)
            }
            if (received_msg == SYS_EVT_DOOR_OPENED) {
                Rte_Write_P_DoorLed_Status(LED_ON); // API chuẩn của ông
                tx_msg.data[0] = 1;
                CanIf_Transmit(&tx_msg);
                UART0_SendString("[Task Door] DOOR OPEN -> LED ON (Feedback Sent)\r\n");
            }
            else if (received_msg == SYS_EVT_DOOR_CLOSED) {
                Rte_Write_P_DoorLed_Status(LED_OFF); // API chuẩn của ông
                tx_msg.data[0] = 0;
                CanIf_Transmit(&tx_msg);
                UART0_SendString("[Task Door] DOOR CLOSED -> LED OFF (Feedback Sent)\r\n");
            }
        }
        osDelay(50);
    }
}

// ============================================================================
// CẦU NỐI VỚI HỆ ĐIỀU HÀNH (RTOS BRIDGE)
// ============================================================================
// Tạo đối tượng Cửa
DoorControl myDoor;

// Vỏ bọc (Wrapper) để file main.cpp và RTOS có thể gọi được Object
extern "C" void DoorControl_Init_Wrapper(void) {
    myDoor.Init();
}

extern "C" void DoorControl_Task_Wrapper(void *argument) {
    (void)argument;
    myDoor.RunTask(); 
}