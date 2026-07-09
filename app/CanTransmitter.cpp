#include "cmsis_os2.h"
#include "can_if.h"
#include "Rte_Types.h"
#include "uart.h"
#include "CanTransmitter.hpp"
extern osMessageQueueId_t canTxQueue;


extern "C" void CanTransmitter_Task_Wrapper(void *argument) {
    (void)argument;
    UART0_SendString("📡 [CAN TX Task] Ready. Waiting for internal events to transmit...\r\n");

    while(1) {
        InternalEvent_t in_event;
        // Lấy sự kiện từ Hộp thư đi (Nếu không có thư, luồng sẽ Block để nhường CPU)
        osStatus_t status = osMessageQueueGet(canTxQueue, &in_event, NULL, osWaitForever);

        if (status == osOK) {
            CanMessage_t tx_msg;
            tx_msg.dlc = 1; 
            tx_msg.data[0] = in_event.event_id;

            // Bộ định tuyến gửi đi (Router Outbound)
            // Đóng gói ID CAN mạng tùy thuộc vào bản chất sự kiện
            if (in_event.event_id == SYS_EVT_DOOR_OPENED || in_event.event_id == SYS_EVT_DOOR_CLOSED) {
                tx_msg.id = 0x100;
            } 
            else if (in_event.event_id == SYS_EVT_WIPER_BTN_PRESSED) {
                tx_msg.id = 0x200;
            } 
            else if (in_event.event_id == SYS_EVT_DIMMER_BTN_PRESSED) {
                tx_msg.id = 0x300;
            }
            else if (in_event.event_id == SYS_EVT_CRASH) {
                tx_msg.id = 0x050; // ID ưu tiên cao dành cho tín hiệu khẩn cấp
            }
            else {
                tx_msg.id = 0x400; // Default
            }

            // Gọi Tầng Dưới (HAL) để truyền đi
            CanIf_Transmit(&tx_msg);
        }
    }
}