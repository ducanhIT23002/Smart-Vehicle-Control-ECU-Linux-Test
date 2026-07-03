#include "InputMonitor.hpp"
#include "Rte_Door.h"
#include "Rte_Wiper.h"
#include "Rte_Light.h"
#include "Rte_Sensor.h"
#include "cmsis_os2.h"
#include "wdg_manager.h" 
#include "uart.h"
#include "gpio.h" 
#include "can_if.h"

// Khởi tạo các giá trị cho cảm biến ngay khi Object vừa sinh ra
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
        SystemEvent_t event_msg;

        CanMessage_t tx_msg;
        tx_msg.dlc = 1; 
        
        uint8_t current_joystick = GPIO_ReadPin(1, 25);
        if (current_joystick == 0 && last_joystick == 1) { 
            SystemEvent_t poison_msg = SYS_EVT_CRASH;
            uint32_t random_target = osKernelGetTickCount() % 3;
            
            tx_msg.data[0] = poison_msg;
            if (random_target == 0) {
                UART0_SendString("[Input Monitor] Joystick: Crashing DOOR (via CAN)!\r\n");
                tx_msg.id = 0x100; 
                CanIf_Transmit(&tx_msg);
            } 	
            else if (random_target == 1) {
                UART0_SendString("[Input Monitor] Joystick: Crashing WIPER (via CAN)!\r\n");
                tx_msg.id = 0x200; 
                CanIf_Transmit(&tx_msg);
            } 
            else if (random_target == 2) {
                UART0_SendString("[Input Monitor] Joystick: Crashing LIGHT (via CAN)!\r\n");
                tx_msg.id = 0x300;
                CanIf_Transmit(&tx_msg);
            }
        }
        last_joystick = current_joystick;

        uint8_t current_door = Rte_Read_DoorStatus();
        if (current_door != last_door) {
            event_msg = (current_door == 1) ? SYS_EVT_DOOR_OPENED : SYS_EVT_DOOR_CLOSED;
            tx_msg.id = 0x100; 
            tx_msg.data[0] = event_msg;
            CanIf_Transmit(&tx_msg);
            last_door = current_door;
        }

        uint8_t current_wiper_sw = Rte_Read_WiperSwitch();
        if (current_wiper_sw == 0 && last_wiper_sw == 1) { 
            event_msg = SYS_EVT_WIPER_BTN_PRESSED;
            tx_msg.id = 0x200;
            tx_msg.data[0] = event_msg;
            CanIf_Transmit(&tx_msg);
        }
        last_wiper_sw = current_wiper_sw;

        uint8_t current_dimmer_sw = Rte_Read_DimmerSwitch();
        if (current_dimmer_sw == 0 && last_dimmer_sw == 1) { 
            event_msg = SYS_EVT_DIMMER_BTN_PRESSED;
            tx_msg.id = 0x300;
            tx_msg.data[0] = event_msg;
            CanIf_Transmit(&tx_msg);
        }
        last_dimmer_sw = current_dimmer_sw;

        EnvLightState_t current_env = Rte_Read_EnvLight();
        if (current_env != last_env) {
            event_msg = (current_env == ENV_DARK) ? SYS_EVT_ENV_DARK : SYS_EVT_ENV_BRIGHT;
            tx_msg.id = 0x300;
            tx_msg.data[0] = event_msg;
            CanIf_Transmit(&tx_msg);
            last_env = current_env;
        }

        osDelay(50);
    }
}

// ============================================================================
// CẦU NỐI RTOS
// ============================================================================
InputMonitor myInput;

extern "C" void InputMonitor_Init_Wrapper(void) {
    myInput.Init();
}

extern "C" void InputMonitor_Task_Wrapper(void *argument) {
    (void)argument;
    myInput.RunTask();
}