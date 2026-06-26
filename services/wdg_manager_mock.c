#include "wdg_manager.h"
#include "cmsis_os2.h"
#include <stdio.h>

void WdgM_Init(void) {
    printf("[WdgM MOCK] Watchdog Manager Initialized.\n");
}

void WdgM_CheckpointReached(uint32_t task_id) {
    // Trong môi trường SiL, tạm thời bỏ qua logic đếm ngược (countdown)
    // Chỉ cần hàm tồn tại để hệ thống không báo lỗi.
}

__NO_RETURN void WdgM_Task(void *argument) {
    (void)argument;
    while(1) {
        osDelay(100);
    }
}