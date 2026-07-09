#include "SecurityFilter.hpp"
#include "uart.h"

SecurityFilter::SecurityFilter() {
    tokens = 10;       // Xô mặc định có 10 thẻ
    loop_counter = 0;
}

bool SecurityFilter::IsFrameAllowed(uint32_t can_id) {
    // 1. WHITELIST: Tín hiệu khẩn cấp (Crash, Túi khí) -> Luôn cho qua!
    if (can_id == 0x050 || can_id == 0x010) {
        return true; 
    }
    
    // 2. RATE LIMIT: Các ID bình thường (Cửa, Đèn...) -> Bắt buộc phải có thẻ
    if (tokens > 0) {
        tokens--; // Thu 1 thẻ
        return true;
    } else {
        // Hết thẻ -> Hacker đang Spam -> Từ chối dịch vụ
        UART0_SendString("🛡️ [Cybersecurity] BLOCKED! Rate limit exceeded. Dropping frame!\r\n");
        return false;
    }
}

void SecurityFilter::UpdateTokens() {
    loop_counter++;
    // Cứ sau 10 vòng lặp (10ms) thì cấp thêm 1 thẻ
    if (loop_counter >= 10) {
        if (tokens < 10) {
            tokens++;
        }
        loop_counter = 0; // Reset bộ đếm
    }
}