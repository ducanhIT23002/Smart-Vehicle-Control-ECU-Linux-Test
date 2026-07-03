#include "CanDispatcher.hpp"
#include "Rte_Types.h"
#include "cmsis_os2.h"
#include "can_if.h"
#include "uart.h"
#include "NvM.hpp"
#include <stdio.h>
extern osMessageQueueId_t doorQueue;
extern osMessageQueueId_t wiperQueue;
extern osMessageQueueId_t lightQueue;

CanDispatcher::CanDispatcher() {}

void CanDispatcher::Init() {}

void CanDispatcher::RunTask() {
    UART0_SendString("📡 [Can Dispatcher] Ready & Listening on CAN Bus...\r\n");

    // ==========================================================
    // THÊM MỚI: BIẾN QUẢN LÝ TOKEN BUCKET (CYBERSECURITY)
    // ==========================================================
    int32_t tokens = 10;         // Xô chứa tối đa 10 thẻ
    uint32_t loop_counter = 0;   // Biến đếm vòng lặp để hồi phục thẻ

    while(1) {
        CanMessage_t rx_msg;

        // Vắt kiệt Buffer, nhưng phải kiểm tra xem có đủ thẻ bài để xử lý không
        while (CanIf_Receive(&rx_msg) == 1) {
            
            // KIỂM TRA BẢO MẬT: Còn thẻ bài không?
            if (tokens > 0) {
                tokens--; // Thu 1 thẻ bài
                
                SystemEvent_t event = (SystemEvent_t)rx_msg.data[0];
                osStatus_t status; 

            switch(rx_msg.id) {
                case 0x100:
                    status = osMessageQueuePut(doorQueue, &event, 0, 0);
                    // CHẨN ĐOÁN: Bắt lỗi nếu Queue từ chối nhận thêm đồ
                    if (status != osOK) {
                        UART0_SendString("🔥 [ERROR/DTC] DOOR Queue Full! Lost Frame!\r\n");
                        myNvM.Inc_QueueFullError(); // Ghi lỗi vào NvM
                    } else {
                        UART0_SendString("📡 [Dispatcher] Forwarding to DOOR Queue\r\n");
                    }
                    break;
                    
                case 0x200:
                    status = osMessageQueuePut(wiperQueue, &event, 0, 0);
                    if (status != osOK) {
                        UART0_SendString("🔥 [ERROR/DTC] WIPER Queue Full! Lost Frame!\r\n");
                        myNvM.Inc_QueueFullError(); // Ghi lỗi vào NvM
                    } else {
                        UART0_SendString("📡 [Dispatcher] Forwarding to WIPER Queue\r\n");
                    }
                    break;
                    
                case 0x300:
                    status = osMessageQueuePut(lightQueue, &event, 0, 0);
                    if (status != osOK) {
                        UART0_SendString("🔥 [ERROR/DTC] LIGHT Queue Full! Lost Frame!\r\n");
                        myNvM.Inc_QueueFullError(); // Ghi lỗi vào NvM
                    } else {
                        UART0_SendString("📡 [Dispatcher] Forwarding to LIGHT Queue\r\n");
                    }
                    break;
                // ==========================================================
                // THÊM MỚI: CỔNG GIAO TIẾP UDS (ISO 14229) CHO THỢ SỬA XE
                // ==========================================================
                case 0x7E0: 
                {
                    CanMessage_t tx_msg;
                    tx_msg.id = 0x7E8; // ID phản hồi chuẩn UDS cho Tester
                    tx_msg.dlc = 8;    // Gói tin UDS thường fix cứng 8 bytes
                    
                    // Byte 0 chứa SID. 0x19 (Hệ thập phân là 25) nghĩa là Read DTC
                    // Byte 0 chứa SID. 
                    if (rx_msg.data[0] == 0x19) { // Lệnh Đọc lỗi (Read DTC)
                        uint16_t error_count = myNvM.Get_QueueFullError();
                        
                        tx_msg.data[0] = 0x59; // Positive Response (0x19 + 0x40)
                        tx_msg.data[1] = (error_count >> 8) & 0xFF; 
                        tx_msg.data[2] = error_count & 0xFF;        
                        
                        CanIf_Transmit(&tx_msg);
                        
                        char log_buf[100];
                        sprintf(log_buf, "🛠️ [UDS] Read DTC (0x19) requested. Found %d errors!\r\n", error_count);
                        UART0_SendString(log_buf);
                    } 
                    else if (rx_msg.data[0] == 0x14) { // Lệnh Xóa lỗi (Clear DTC)
                        // 1. Gọi hàm tẩy não NvM (Xóa bỏ - 消去します - しょうきょします)
                        myNvM.Clear_QueueFullError();
                        
                        // 2. Đóng gói câu trả lời thành công
                        tx_msg.data[0] = 0x54; // Positive Response (0x14 + 0x40)
                        tx_msg.data[1] = 0x00; // Trả về data rỗng vì đã xóa sạch
                        tx_msg.data[2] = 0x00;
                        
                        // 3. Bắn phản hồi về cho thợ xe
                        CanIf_Transmit(&tx_msg);
                        UART0_SendString("🛠️ [UDS] Clear DTC (0x14) requested. Memory wiped!\r\n");
                    }
                    break;
                }
                default:
                    // Bắt luôn lỗi nếu ID không nằm trong danh sách thiết kế
                    UART0_SendString("⚠️ [WARNING] Unknown CAN ID received! Ignored.\r\n");
                    break;
            }
            } 
            else {
                // TỪ CHỐI DỊCH VỤ (Drop Frame): Xô đã cạn, Hacker đang SPAM!
                UART0_SendString("🛡️ [Cybersecurity] BLOCKED! Rate limit exceeded. Dropping frame!\r\n");
            }
        }
        
        // HỒI PHỤC THẺ BÀI: Cứ sau 10 vòng lặp (10ms) thì cấp thêm 1 thẻ
        loop_counter++;
        if (loop_counter >= 10) {
            if (tokens < 10) {
                tokens++; // Thêm 1 thẻ vào xô
            }
            loop_counter = 0; // Reset bộ đếm
        }

        // Ngủ 1ms nhường CPU
        osDelay(1);
    }
}

// ============================================================================
// CẦU NỐI RTOS
// ============================================================================
CanDispatcher myDispatcher;

extern "C" void CanDispatcher_Init_Wrapper(void) {
    myDispatcher.Init();
}

extern "C" void CanDispatcher_Task_Wrapper(void *argument) {
    (void)argument;
    myDispatcher.RunTask();
}






