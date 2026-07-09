#include "InputMonitor.hpp"
#include "Rte_Door.h"
#include "Rte_Wiper.h"
#include "Rte_Light.h"
#include "Rte_Sensor.h"
#include "cmsis_os2.h"
#include "wdg_manager.h" 
#include "uart.h"
#include "gpio.h" 
#include "CanTransmitter.hpp"
// ĐÃ XÓA #include "can_if.h" - InputMonitor không còn biết mạng CAN là gì nữa!

// Khai báo hộp thư đi (Sẽ được tạo ở file main)
extern osMessageQueueId_t canTxQueue;


InputMonitor::InputMonitor() {
    last_door = 1;      
    last_wiper_sw = 1;  
    last_dimmer_sw = 1; 
    last_env = ENV_BRIGHT; 
    last_joystick = 1; 
}

void InputMonitor::Init() {
    GPIO_SetDir(1, 25, 0);
}

void InputMonitor::RunTask() {
    while(1) {
        WdgM_CheckpointReached(WDG_INPUT_TASK_ID);
        InternalEvent_t out_event;

        // 1. Quét Joystick (Tiêm lỗi Crash)
        uint8_t current_joystick = GPIO_ReadPin(1, 25);
        if (current_joystick == 0 && last_joystick == 1) { 
            out_event.event_id = SYS_EVT_CRASH;
            out_event.payload = 0xFF; // Dữ liệu rác
            osMessageQueuePut(canTxQueue, &out_event, 0, 0); // Vứt vào Queue, xong việc!
        }
        last_joystick = current_joystick;

        // 2. Quét Trạng thái Cửa
        uint8_t current_door = Rte_Read_DoorStatus();
        if (current_door != last_door) {
            out_event.event_id = (current_door == 1) ? SYS_EVT_DOOR_OPENED : SYS_EVT_DOOR_CLOSED;
            out_event.payload = 0x01;
            osMessageQueuePut(canTxQueue, &out_event, 0, 0);
            last_door = current_door;
        }

        // 3. Quét Nút Gạt mưa
        uint8_t current_wiper_sw = Rte_Read_WiperSwitch();
        if (current_wiper_sw == 0 && last_wiper_sw == 1) { 
            out_event.event_id = SYS_EVT_WIPER_BTN_PRESSED;
            out_event.payload = 0x02;
            osMessageQueuePut(canTxQueue, &out_event, 0, 0);
        }
        last_wiper_sw = current_wiper_sw;

        // 4. Quét Nút Dimmer (Đèn trong xe)
        uint8_t current_dimmer_sw = Rte_Read_DimmerSwitch();
        if (current_dimmer_sw == 0 && last_dimmer_sw == 1) { 
            out_event.event_id = SYS_EVT_DIMMER_BTN_PRESSED;
            out_event.payload = 0x03;
            osMessageQueuePut(canTxQueue, &out_event, 0, 0);
        }
        last_dimmer_sw = current_dimmer_sw;

        osDelay(50);
    }
}

// CẦU NỐI RTOS
InputMonitor myInput;
extern "C" void InputMonitor_Init_Wrapper(void) { myInput.Init(); }
extern "C" void InputMonitor_Task_Wrapper(void *argument) { myInput.RunTask(); }