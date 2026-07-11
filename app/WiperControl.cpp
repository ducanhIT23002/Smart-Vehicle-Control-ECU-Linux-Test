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

    int wipe_timer = 0; // Bộ đếm cho FR-003

    while(1) {
        WdgM_CheckpointReached(WDG_WIPER_TASK_ID);
        SystemEvent_t received_msg;
        
        // 1. EVENT-DRIVEN: XỬ LÝ LỆNH TỪ USER
        if (osMessageQueueGet(wiperQueue, &received_msg, NULL, 0) == osOK) {
            CanMessage_t tx_msg;
            tx_msg.dlc = 1;
            tx_msg.id = 0x201;

            if (received_msg == SYS_EVT_CRASH) {
                UART0_SendString("💥 [FR-002] CRASH RECEIVED! Stopping wiper.\r\n");
                // Đã gỡ bỏ while(1) {} để không cắn Watchdog
                current_wiper_mode = WIPER_OFF;
                Rte_Write_WiperMode(current_wiper_mode);
                wipe_timer = 0;
                Rte_Write_WiperActive_Flag(0);
            }
            else if (received_msg == SYS_EVT_WIPER_BTN_PRESSED) {
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
                    
                    // FR-003: Tắt gạt mưa thì xóa cờ và reset đếm
                    wipe_timer = 0;
                    Rte_Write_WiperActive_Flag(0);
                }
            }
        }

        // 2. DATA-DRIVEN: ĐẾM GIỜ GẠT MƯA (FR-003)
        if (current_wiper_mode == WIPER_INT) {
            wipe_timer++;
            // 100 chu kỳ * 50ms = 5000ms = 5 giây
            if (wipe_timer == 100) {
                UART0_SendString("🌧️ [FR-003] Heavy Rain Detected! Setting Flag for Lights!\r\n");
                Rte_Write_WiperActive_Flag(1); // Dán cờ lên Rte_Wiper
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