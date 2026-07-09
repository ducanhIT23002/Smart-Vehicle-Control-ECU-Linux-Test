#include "CanDispatcher.hpp"
#include "Rte_Types.h"
#include "cmsis_os2.h"
#include "can_if.h"
#include "uart.h"
#include "NvM.hpp"
#include "SecurityFilter.hpp"
#include "DiagnosticService.hpp"

extern osMessageQueueId_t doorQueue;
extern osMessageQueueId_t wiperQueue;
extern osMessageQueueId_t lightQueue;

// Khởi tạo các module vệ tinh
SecurityFilter mySecurity;
DiagnosticService myDiag;

CanDispatcher::CanDispatcher() {}
void CanDispatcher::Init() {}

void CanDispatcher::RunTask() {
    UART0_SendString("📡 [Can Dispatcher] Ready & Listening on CAN Bus...\r\n");

    while(1) {
        CanMessage_t rx_msg;

        // Vắt kiệt Buffer TCP/IP
        while (CanIf_Receive(&rx_msg) == 1) {
            
            // 🛡️ BƯỚC 1: BẢO MẬT (Qua trạm kiểm soát)
            if (!mySecurity.IsFrameAllowed(rx_msg.id)) {
                continue; // Hacker! Bỏ qua gói tin này ngay lập tức.
            }

            // 🛠️ BƯỚC 2: CHẨN ĐOÁN (Phân luồng UDS)
            if (rx_msg.id == 0x7E0) {
                myDiag.ProcessRequest(&rx_msg); // Nhờ bác sĩ khám
                continue; // Xong việc, không đẩy vào Queue
            }
            
            // 📡 BƯỚC 3: ĐỊNH TUYẾN NGHIỆP VỤ (Routing & Priority)
            SystemEvent_t event = (SystemEvent_t)rx_msg.data[0];
            osStatus_t status; 

            switch(rx_msg.id) {
                case 0x100:
                    // Tham số thứ 3 là Priority. Cửa được ưu tiên cao hơn (Priority 2)
                    status = osMessageQueuePut(doorQueue, &event, 2, 0); 
                    if (status != osOK) myNvM.Inc_QueueFullError();
                    break;
                    
                case 0x200:
                    status = osMessageQueuePut(wiperQueue, &event, 1, 0); // Priority 1
                    if (status != osOK) myNvM.Inc_QueueFullError();
                    break;
                    
                case 0x300:
                    status = osMessageQueuePut(lightQueue, &event, 1, 0); // Priority 1
                    if (status != osOK) myNvM.Inc_QueueFullError();
                    break;
                    
                case 0x050:
                case 0x010:
                    UART0_SendString("🚨 [Dispatcher] High-Priority Safety Event Received!\r\n");
                    // Tương lai sẽ đẩy vào một Queue khẩn cấp
                    break;

                default:
                    UART0_SendString("⚠️ [WARNING] Unknown CAN ID received! Ignored.\r\n");
                    break;
            }
        }
        
        // 🔋 BƯỚC 4: BẢO TRÌ (Hồi phục thẻ bài Token Bucket)
        mySecurity.UpdateTokens();

        osDelay(1); // Ngủ 1ms
    }
}

// CẦU NỐI RTOS
CanDispatcher myDispatcher;
extern "C" void CanDispatcher_Init_Wrapper(void) { myDispatcher.Init(); }
extern "C" void CanDispatcher_Task_Wrapper(void *argument) { myDispatcher.RunTask(); }