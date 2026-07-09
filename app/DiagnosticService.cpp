#include "DiagnosticService.hpp"
#include "NvM.hpp"
#include "uart.h"
#include <stdio.h>

DiagnosticService::DiagnosticService() {}

void DiagnosticService::ProcessRequest(const CanMessage_t* rx_msg) {
    CanMessage_t tx_msg;
    tx_msg.id = 0x7E8; // ID phản hồi chuẩn UDS cho Tester
    tx_msg.dlc = 8;    
    
    // Lệnh Đọc lỗi (Read DTC)
    if (rx_msg->data[0] == 0x19) { 
        uint16_t error_count = myNvM.Get_QueueFullError();
        
        tx_msg.data[0] = 0x59; // Positive Response
        tx_msg.data[1] = (error_count >> 8) & 0xFF; 
        tx_msg.data[2] = error_count & 0xFF;        
        
        CanIf_Transmit(&tx_msg);
        
        char log_buf[100];
        sprintf(log_buf, "🛠️ [UDS] Read DTC (0x19) requested. Found %d errors!\r\n", error_count);
        UART0_SendString(log_buf);
    } 
    // Lệnh Xóa lỗi (Clear DTC)
    else if (rx_msg->data[0] == 0x14) { 
        myNvM.Clear_QueueFullError();
        
        tx_msg.data[0] = 0x54; // Positive Response
        tx_msg.data[1] = 0x00; 
        tx_msg.data[2] = 0x00;
        
        CanIf_Transmit(&tx_msg);
        UART0_SendString("🛠️ [UDS] Clear DTC (0x14) requested. Memory wiped!\r\n");
    }
}