#include "cmsis_os2.h"
#include "uart.h"
#include "can_if.h"
#include "wdg_manager.h"
#include "EcuM.h"
#include "Rte_Types.h"
#include <stdio.h>
// Khai báo các Module
#include "DoorControl.hpp"
#include "WiperControl.hpp"
#include "LightControl.hpp"
#include "CanDispatcher.hpp"
#include "InputMonitor.hpp"

// Báo cho hệ thống biết về các Task
extern void DoorControl_Task(void *argument);
extern void WiperControl_Task(void *argument);
extern void LightControl_Task(void *argument);
extern void CanDispatcher_Task(void *argument);
extern void InputMonitor_Task(void *argument);

// Khởi tạo các Biến toàn cục (グローバル変数)
osMessageQueueId_t doorQueue;
osMessageQueueId_t wiperQueue;
osMessageQueueId_t lightQueue;
osMutexId_t uartMutex;

osThreadId_t doorTask_id, wiperTask_id, lightTask_id, dispatcherTask_id, inputTask_id;
osThreadId_t wdgTask_id;

int main(void) {

    // Vô hiệu hóa (無効にします - むこうにします)
    // 例文: バッファリングを無効(むこう)にします。
    // (Vô hiệu hóa bộ đệm.)
    // Ép stdout không giữ lại bộ đệm (No Buffer), có chữ nào đẩy thẳng ra ống Pipe ngay lập tức
    setvbuf(stdout, NULL, _IONBF, 0);

    // 1. Giả lập khởi tạo phần cứng
    UART0_Init(9600);
    UART0_SendString("\r\n=== Smart BCM (SiL Edition) Booting ===\r\n");

    EcuM_Init();
    CanIf_Init();
    WdgM_Init();

    // 2. Khởi tạo các Ứng dụng
    DoorControl_Init();
    WiperControl_Init();
    LightControl_Init();
    InputMonitor_Init();

    // 3. Khởi tạo Hệ điều hành đa nhiệm
    osKernelInitialize();
    uartMutex = osMutexNew(NULL);

    doorQueue  = osMessageQueueNew(5, sizeof(SystemEvent_t), NULL);
    wiperQueue = osMessageQueueNew(5, sizeof(SystemEvent_t), NULL);
    lightQueue = osMessageQueueNew(5, sizeof(SystemEvent_t), NULL);

    // 4. Bật các Luồng (Tasks)
    doorTask_id       = osThreadNew(DoorControl_Task, NULL, NULL);
    wiperTask_id      = osThreadNew(WiperControl_Task, NULL, NULL);
    lightTask_id      = osThreadNew(LightControl_Task, NULL, NULL);
    dispatcherTask_id = osThreadNew(CanDispatcher_Task, NULL, NULL);
    inputTask_id      = osThreadNew(InputMonitor_Task, NULL, NULL);
    wdgTask_id        = osThreadNew(WdgM_Task, NULL, NULL);

    UART0_SendString("Starting OS Kernel...\r\n");
    
    // 5. Chạy vòng lặp hệ điều hành
    osKernelStart(); 

    while(1) {}
    return 0;
}