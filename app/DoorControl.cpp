#include "DoorControl.hpp"
#include "Rte_Door.h"
#include "Rte_Types.h"
#include "cmsis_os2.h"
#include "uart.h"
#include "can_if.h"
#include "wdg_manager.h"
extern osMessageQueueId_t doorQueue;

void DoorControl_Init(void) { }

__NO_RETURN void DoorControl_Task(void *argument) {
    (void)argument;
    UART0_SendString("[Task Door] Ready & Waiting...\r\n");

    while(1) {
        WdgM_CheckpointReached(WDG_DOOR_TASK_ID);
        SystemEvent_t received_msg;
        if (osMessageQueueGet(doorQueue, &received_msg, NULL, 500) == osOK) {
            CanMessage_t tx_msg;
            tx_msg.dlc = 1;
            tx_msg.id = 0x101;

            if (received_msg == SYS_EVT_CRASH) {
                UART0_SendString("[Task Door] CRASH RECEIVED\r\n");
                while(1) {}
            }
            if (received_msg == SYS_EVT_DOOR_OPENED) {
                Rte_Write_P_DoorLed_Status(LED_ON);
                tx_msg.data[0] = 1;
                CanIf_Transmit(&tx_msg);
                UART0_SendString("[Task Door] DOOR OPEN -> LED ON (Feedback Sent)\r\n");
            }
            else if (received_msg == SYS_EVT_DOOR_CLOSED) {
                Rte_Write_P_DoorLed_Status(LED_OFF);
                tx_msg.data[0] = 0;
                CanIf_Transmit(&tx_msg);
                UART0_SendString("[Task Door] DOOR CLOSED -> LED OFF (Feedback Sent)\r\n");
            }
        }
    }
}