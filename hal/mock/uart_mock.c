#include "uart.h"
#include <stdio.h> // Thư viện chứa hàm printf của PC

void UART0_Init(uint32_t baudrate) {
    // Trên Linux, phần cứng là ảo nên không cần cài đặt tốc độ baudrate thực sự
    printf("[UART MOCK] System Initialized with baudrate: %d\n", baudrate);
}

void UART0_SendString(const char *str) {
    // Chuyển lệnh gửi qua cổng UART thành lệnh in ra màn hình Terminal
    printf("%s", str);
}