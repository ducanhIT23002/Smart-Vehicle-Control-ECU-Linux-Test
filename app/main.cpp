#include "cmsis_os2.h"
#include "uart.h"
#include "can_if.h"
#include "wdg_manager.h"
#include "EcuM.h"
#include "Rte_Types.h"
#include <stdio.h>

// ----------------------------------------------------------------------------
// KHAI BÁO MODULE
// (Các file .hpp này đã tự động chứa sẵn các hàm _Wrapper bên trong)
// ----------------------------------------------------------------------------
#include "DoorControl.hpp"
#include "WiperControl.hpp"
#include "LightControl.hpp"
#include "CanDispatcher.hpp"
#include "InputMonitor.hpp"
#include "NvM.hpp"
#include "CanTransmitter.hpp"
// ----------------------------------------------------------------------------
// BIẾN TOÀN CỤC (Global Variables)
// ----------------------------------------------------------------------------
osMessageQueueId_t doorQueue;
osMessageQueueId_t wiperQueue;
osMessageQueueId_t lightQueue;
osMessageQueueId_t canTxQueue;
osMutexId_t uartMutex;

osThreadId_t doorTask_id, wiperTask_id, lightTask_id, dispatcherTask_id, inputTask_id;
osThreadId_t wdgTask_id,canTxTask_id;

// ============================================================================
// HÀM MAIN CHÍNH
// ============================================================================
int main(void) {

    // Ép stdout không giữ lại bộ đệm (No Buffer) để đẩy ra Pipe ngay lập tức cho Python đọc
    setvbuf(stdout, NULL, _IONBF, 0);

    // 1. Giả lập khởi tạo phần cứng
    UART0_Init(9600);
    UART0_SendString("\r\n=== Smart BCM (SiL Edition) Booting ===\r\n");

    EcuM_Init();
    CanIf_Init();
    WdgM_Init();
    myNvM.Init();
    
    // 2. Khởi tạo các Ứng dụng (Gọi qua vỏ bọc C++ Wrapper)
    DoorControl_Init_Wrapper();
    WiperControl_Init_Wrapper();
    LightControl_Init_Wrapper();
    CanDispatcher_Init_Wrapper(); 
    InputMonitor_Init_Wrapper();

    // 3. Khởi tạo Hệ điều hành đa nhiệm
    osKernelInitialize();
    uartMutex = osMutexNew(NULL);

    doorQueue  = osMessageQueueNew(5, sizeof(SystemEvent_t), NULL);
    wiperQueue = osMessageQueueNew(5, sizeof(SystemEvent_t), NULL);
    lightQueue = osMessageQueueNew(5, sizeof(SystemEvent_t), NULL);
    canTxQueue = osMessageQueueNew(10, sizeof(InternalEvent_t), NULL);
    if (canTxQueue == NULL) {
        UART0_SendString("🔥 [ERROR] Failed to create canTxQueue!\r\n");
    }
    // 4. Bật các Luồng (Tasks)
    doorTask_id       = osThreadNew(DoorControl_Task_Wrapper, NULL, NULL);
    wiperTask_id      = osThreadNew(WiperControl_Task_Wrapper, NULL, NULL);
    lightTask_id      = osThreadNew(LightControl_Task_Wrapper, NULL, NULL);
    dispatcherTask_id = osThreadNew(CanDispatcher_Task_Wrapper, NULL, NULL);
    inputTask_id      = osThreadNew(InputMonitor_Task_Wrapper, NULL, NULL);
    wdgTask_id        = osThreadNew(WdgM_Task, NULL, NULL);
    canTxTask_id      = osThreadNew(CanTransmitter_Task_Wrapper, NULL, NULL);
    UART0_SendString("Starting OS Kernel...\r\n");
    
    // 5. Chạy vòng lặp hệ điều hành
    osKernelStart(); 

    while(1) {}
    return 0;
}