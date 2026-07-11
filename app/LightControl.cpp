#include "LightControl.hpp"
#include "Rte_Light.h"
#include "Rte_Types.h"
#include "Rte_Wiper.h" // 🔴 Thêm thư viện để đọc trạng thái Gạt mưa
#include "cmsis_os2.h"
#include "uart.h"
#include "can_if.h"
#include "wdg_manager.h"

extern osMessageQueueId_t lightQueue;

LightControl::LightControl() {
    is_interior_light_on = 0;
}

void LightControl::Init() {
    Rte_Write_Headlight(LED_OFF);
}

void LightControl::RunTask() {
    UART0_SendString("[Task Light] Ready & Waiting for Events...\r\n");

    bool is_headlight_on_by_rain = false; // Biến trạng thái cho FR-003

    while(1) {
        WdgM_CheckpointReached(WDG_LIGHT_TASK_ID);
        SystemEvent_t received_msg;
        
        // 1. EVENT-DRIVEN: XỬ LÝ LỆNH TỪ MẠNG
        osStatus_t status = osMessageQueueGet(lightQueue, &received_msg, NULL, 0);

        if (status == osOK) {
            CanMessage_t tx_msg;
            tx_msg.dlc = 1;
            tx_msg.id = 0x301;

            switch (received_msg) {
                case SYS_EVT_CRASH:
                    UART0_SendString("💥 [FR-002] CRASH RECEIVED! Hazard Light ON!\r\n");
                    // Đã gỡ bỏ while(1) { osDelay(1000); }
                    break;
                case SYS_EVT_ENV_DARK:
                    Rte_Write_Headlight(LED_ON);
                    tx_msg.data[0] = 0x11;
                    CanIf_Transmit(&tx_msg);
                    UART0_SendString("[Task Light] Dark -> Headlight ON (Feedback Sent)\r\n");
                    break;
                case SYS_EVT_ENV_BRIGHT:
                    // Chỉ tắt nếu trời sáng và không có mưa to
                    if (!is_headlight_on_by_rain) {
                        Rte_Write_Headlight(LED_OFF);
                        tx_msg.data[0] = 0x10;
                        CanIf_Transmit(&tx_msg);
                        UART0_SendString("[Task Light] Bright -> Headlight OFF (Feedback Sent)\r\n");
                    }
                    break;
                case SYS_EVT_DIMMER_BTN_PRESSED:
                    if (is_interior_light_on == 0) {
                        Rte_Call_LightFadeIn();
                        is_interior_light_on = 1;
                        tx_msg.data[0] = 0x21;
                        CanIf_Transmit(&tx_msg);
                        UART0_SendString("[Task Light] Dimmer ON (Feedback Sent)\r\n");
                    } else {
                        Rte_Call_LightFadeOut();
                        is_interior_light_on = 0;
                        tx_msg.data[0] = 0x20;
                        CanIf_Transmit(&tx_msg);
                        UART0_SendString("[Task Light] Dimmer OFF (Feedback Sent)\r\n");
                    }
                    break;
                default:
                    break;
            }
        }

        // 2. DATA-DRIVEN: LIÊN KẾT ĐÈN - GẠT MƯA (FR-003)
        uint8_t wiper_flag = Rte_Read_WiperActive_Flag();
        
        if (wiper_flag == 1 && !is_headlight_on_by_rain) {
            Rte_Write_Headlight(LED_ON);
            UART0_SendString("🚨 [FR-003] Wiper-Light Sync: Heavy Rain -> AUTO HEADLIGHT ON!\r\n");
            is_headlight_on_by_rain = true;
        } 
        else if (wiper_flag == 0 && is_headlight_on_by_rain) {
            Rte_Write_Headlight(LED_OFF);
            UART0_SendString("⛅ [FR-003] Rain Stopped -> AUTO HEADLIGHT OFF!\r\n");
            is_headlight_on_by_rain = false;
        }

        osDelay(50);
    }
}

LightControl myLight;

extern "C" void LightControl_Init_Wrapper(void) {
    myLight.Init();
}

extern "C" void LightControl_Task_Wrapper(void *argument) {
    (void)argument;
    myLight.RunTask(); 
}