#ifndef CMSIS_OS2_H
#define CMSIS_OS2_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NO_RETURN
#define __NO_RETURN
#endif

#define osWaitForever 0xFFFFFFFFU


// Trạng thái (ステータス - suteetasu) trả về của hệ thống
typedef enum {
    osOK = 0,               // Thành công (成功 - seikou)
    osError = -1,           // Lỗi (エラー - eraa)
    osErrorTimeout = -2,    // Hết thời gian chờ (タイムアウト - taimuauto)
    osErrorResource = -3    // Lỗi tài nguyên (リソース - risoosu)
} osStatus_t;

// Định nghĩa các kiểu con trỏ (ポインタ - pointa) đại diện cho các đối tượng OS
typedef void *osThreadId_t;       // ID của Luồng/Tác vụ (スレッド - sureddo / タスク - tasuku)
typedef void *osMessageQueueId_t; // ID của Hàng đợi bản tin (メッセージキュー - messeeji kyuu)
typedef void *osMutexId_t;        // ID của Khóa bảo vệ đa luồng (排他制御 - haita seigyo / ミューテックス - myuutekkusu)

// 1. Quản lý Kernel (カーネル管理 - kaaneru kanri)
osStatus_t osKernelInitialize(void); // Khởi tạo (初期化 - shokika)
osStatus_t osKernelStart(void);      // Bắt đầu chạy (開始 - kaishi)
uint32_t osKernelGetTickCount(void); // Lấy thời gian đếm của hệ thống

// 2. Quản lý Luồng (スレッド管理 - sureddo kanri)
// Tạo một luồng mới
osThreadId_t osThreadNew(void (*func)(void *), void *argument, const void *attr);

// 3. Quản lý Hàng đợi (キュー管理 - kyuu kanri)
// Tạo hàng đợi mới
osMessageQueueId_t osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const void *attr);
// Gửi bản tin (送信 - soushin) vào hàng đợi
osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);
// Nhận bản tin (受信 - jushin) từ hàng đợi
osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);

// 4. Quản lý Khóa Mutex (ミューテックス管理 - myuutekkusu kanri)
osMutexId_t osMutexNew(const void *attr);
// Lấy/Chiếm quyền khóa (取得 - shutoku)
osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout);
// Nhả/Giải phóng khóa (解放 - kaihou)
osStatus_t osMutexRelease(osMutexId_t mutex_id);

// 5. Hàm chờ thời gian (待機関数 - taiki kansuu)
// Trễ/Dừng luồng hiện tại (遅延 - chien)
osStatus_t osDelay(uint32_t ticks);

#ifdef __cplusplus
}
#endif

#endif /* CMSIS_OS2_H */