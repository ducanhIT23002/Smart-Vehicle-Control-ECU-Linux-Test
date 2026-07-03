#include "WiperControl.hpp"
#include "Rte_Wiper.h"
#include "Rte_Types.h"
#include "cmsis_os2.h"
#include "uart.h"
#include "can_if.h"
#include "wdg_manager.h"

extern osMessageQueueId_t wiperQueue;

// ============================================================================
// CONSTRUCTOR & INIT
// ============================================================================
WiperControl::WiperControl() {
    current_wiper_mode = WIPER_OFF;
}

void WiperControl::Init() {
    Rte_Write_WiperMode(current_wiper_mode);
}

// ============================================================================
// TASK CHÍNH
// ============================================================================
void WiperControl::RunTask() {
    UART0_SendString("[Task Wiper] Ready & Waiting...\r\n");

    while(1) {
        WdgM_CheckpointReached(WDG_WIPER_TASK_ID);
        SystemEvent_t received_msg;
        if (osMessageQueueGet(wiperQueue, &received_msg, NULL, 0) == osOK) {
            CanMessage_t tx_msg;
            tx_msg.dlc = 1;
            tx_msg.id = 0x201;

            if (received_msg == SYS_EVT_CRASH) {
                UART0_SendString("[Task Wiper] CRASH RECEIVED\r\n");
                while(1) {}
            }
            if (received_msg == SYS_EVT_WIPER_BTN_PRESSED) {
                current_wiper_mode = (current_wiper_mode == WIPER_OFF) ? WIPER_INT : WIPER_OFF;
                Rte_Write_WiperMode(current_wiper_mode);

                if(current_wiper_mode == WIPER_INT) {
                    tx_msg.data[0] = 1;
                    CanIf_Transmit(&tx_msg);
                    UART0_SendString("[Task Wiper] Mode INT (Feedback Sent)\r\n");
                } else {
                    tx_msg.data[0] = 0;
                    CanIf_Transmit(&tx_msg);
                    UART0_SendString("[Task Wiper] Mode OFF (Feedback Sent)\r\n");
                }
            }
        }
        osDelay(50);
    }
}

// ============================================================================
// CẦU NỐI RTOS
// ============================================================================
WiperControl myWiper;

extern "C" void WiperControl_Init_Wrapper(void) {
    myWiper.Init();
}

extern "C" void WiperControl_Task_Wrapper(void *argument) {
    (void)argument;
    myWiper.RunTask(); 
}