#include "wdg_manager.h"
#include "cmsis_os2.h"
#include "uart.h"   // Thêm thư viện này để dùng UART0_SendString
#include <stdio.h>
#include <stdlib.h> // Thêm thư viện này để dùng hàm exit()

// Khai báo hàm phần cứng để Fail-Safe có thể gọi chéo vượt mặt HĐH
extern void Rte_Write_Headlight(unsigned char state);

// ==========================================================
// CẤU TRÚC THEO DÕI TASK (監視 - かんし - Giám sát)
// ==========================================================
typedef struct {
    uint32_t task_id;
    int32_t alive_counter; // Bình oxy của Task
} WdgM_Checkpoint_t;



// Khởi tạo danh sách 4 Task (Input, Door, Wiper, Light) với "bình oxy" 50 nhịp (5 giây)
#define MAX_TASKS 4
static WdgM_Checkpoint_t checkpoints[MAX_TASKS] = {
    {WDG_INPUT_TASK_ID, 50}, 
    {WDG_DOOR_TASK_ID, 50}, 
    {WDG_WIPER_TASK_ID, 50}, 
    {WDG_LIGHT_TASK_ID, 50}  // Task Đèn mang ID số 4
};

// ... (Các hàm WdgM_Init, WdgM_CheckpointReached, WdgM_Task giữ nguyên) ...

void WdgM_Init(void) {
    UART0_SendString("[WdgM MOCK] Watchdog Manager Initialized.\r\n");
}

// Hàm này được các Task gọi liên tục (Ví dụ: dòng 23 trong LightControl.cpp của ông)
void WdgM_CheckpointReached(uint32_t task_id) {
    // Task báo cáo còn sống -> Bơm lại đầy "bình oxy" (回復します - かいふくします - Hồi phục)
    for (int i = 0; i < MAX_TASKS; i++) {
        if (checkpoints[i].task_id == task_id) {
            checkpoints[i].alive_counter = 50; 
            break;
        }
    }
}

// Luồng Watchdog chạy ngầm độc lập do Hệ điều hành quản lý
__NO_RETURN void WdgM_Task(void *argument) {
    (void)argument;
    while(1) {
        osDelay(100); // 100ms kiểm tra 1 lần

        for (int i = 0; i < MAX_TASKS; i++) {
            checkpoints[i].alive_counter--; // Trừ dần bình oxy của từng Task

            // Nếu cạn oxy -> Task đã chết lâm sàng (Deadlock)
            if (checkpoints[i].alive_counter <= 0) {
                char buf[100];
                sprintf(buf, "💀 [WdgM] FATAL ERROR! Task %d is DEAD!\r\n", checkpoints[i].task_id);
                UART0_SendString(buf);

                // ==========================================================
                // CƠ CHẾ FAIL-SAFE (AN TOÀN CHỨC NĂNG ISO 26262)
                // ==========================================================
                UART0_SendString("🚨 [FAIL-SAFE] KÍCH HOẠT AN TOÀN! ÉP BẬT SÁNG ĐÈN PHA NGAY LẬP TỨC!\r\n");
                Rte_Write_Headlight(1); // Ép phần cứng bật đèn (1 = LED_ON)

                UART0_SendString("🔄 [WdgM] Resetting System...\r\n");
                exit(EXIT_FAILURE); // Sập nguồn hệ thống
            }
        }
    }
}