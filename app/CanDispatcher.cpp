#include "CanDispatcher.hpp"
#include "Rte_Types.h"
#include "cmsis_os2.h"
#include "can_if.h"
#include "uart.h"

extern osMessageQueueId_t doorQueue;
extern osMessageQueueId_t wiperQueue;
extern osMessageQueueId_t lightQueue;

void CanDispatcher_Init(void) {
}

__NO_RETURN void CanDispatcher_Task(void *argument) {
    (void)argument;
    UART0_SendString("[Can Dispatcher] Ready & Listening on CAN Bus...\r\n");

    while(1) {
        CanMessage_t rx_msg;

        if (CanIf_Receive(&rx_msg) == 1) {
            SystemEvent_t event = (SystemEvent_t)rx_msg.data[0];

            switch(rx_msg.id) {
                case 0x100:
                    UART0_SendString("[Dispatcher] Forwarding to DOOR Queue\r\n");
                    osMessageQueuePut(doorQueue, &event, 0, 0);
                    break;
                case 0x200:
                    UART0_SendString("[Dispatcher] Forwarding to WIPER Queue\r\n");
                    osMessageQueuePut(wiperQueue, &event, 0, 0);
                    break;
                case 0x300:
                    UART0_SendString("[Dispatcher] Forwarding to LIGHT Queue\r\n");
                    osMessageQueuePut(lightQueue, &event, 0, 0);
                    break;
                default:
                    break;
            }
        }

        osDelay(20);
    }
}