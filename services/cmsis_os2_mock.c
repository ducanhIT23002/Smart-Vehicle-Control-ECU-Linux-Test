#include "cmsis_os2.h"
#include <pthread.h> // Thư viện (ライブラリ - らいぶらり - thư viện) đa luồng của Linux
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// --- 1. KERNEL (Nhân hệ điều hành) ---
osStatus_t osKernelInitialize(void) {
    // Khởi tạo (初期化します - しょきかします)
    return osOK; 
}

osStatus_t osKernelStart(void) {
    // Vòng lặp vô hạn (無限ループ - むげんるーぷ - vòng lặp vô hạn) giữ cho chương trình sống
    while(1) {
        usleep(1000000); 
    }
    return osOK;
}

uint32_t osKernelGetTickCount(void) {
    return 0; // Tạm thời bỏ qua
}

// --- 2. THREAD (Quản lý luồng) ---
osThreadId_t osThreadNew(void (*func)(void *), void *argument, const void *attr) {
    // Cấp phát (割り当てる - わりあてる - cấp phát) bộ nhớ (メモリ - めもり - bộ nhớ) cho luồng
    pthread_t *thread = malloc(sizeof(pthread_t));
    
    // Tạo mới (作成 - さくせい - tạo mới) một luồng Linux thực sự
    if (pthread_create(thread, NULL, (void *(*)(void *))func, argument) != 0) {
        free(thread);
        // Báo lỗi (エラー - えらー - lỗi)
        return NULL; 
    }
    return (osThreadId_t)thread;
}

osStatus_t osDelay(uint32_t ticks) {
    // Tạm dừng (一時停止 - いちじていし - tạm dừng) luồng hiện tại
    usleep(ticks * 1000); // Chuyển mili-giây sang micro-giây
    return osOK;
}

// --- 3. MUTEX (Khóa bảo vệ) ---
typedef struct {
    pthread_mutex_t mutex;
} MockMutex_t;

osMutexId_t osMutexNew(const void *attr) {
    MockMutex_t *m = malloc(sizeof(MockMutex_t));
    pthread_mutex_init(&m->mutex, NULL);
    return (osMutexId_t)m;
}

osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout) {
    MockMutex_t *m = (MockMutex_t *)mutex_id;
    // Chiếm quyền/Khóa (ロック - ろっく - khóa) 
    pthread_mutex_lock(&m->mutex);
    return osOK;
}

osStatus_t osMutexRelease(osMutexId_t mutex_id) {
    MockMutex_t *m = (MockMutex_t *)mutex_id;
    // Giải phóng (解放 - かいほう - giải phóng) khóa
    pthread_mutex_unlock(&m->mutex);
    return osOK;
}

// ============================================================================
// 4. MESSAGE QUEUE (Hàng đợi tin nhắn - KHÔNG DÙNG MALLOC)
// ============================================================================
typedef struct {
    void *data;
    uint32_t msg_size;
    uint32_t msg_count;
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty; 
} MockQueue_t;

// Cấp phát tĩnh (静的割り当て - せいてきわりあて)
// 例文: 配列(はいれつ)を使(つか)ってメモリを静的(せいてき)に割(わ)り当(あ)てます。
// (Cấp phát bộ nhớ một cách tĩnh bằng cách sử dụng mảng.)
#define MAX_QUEUES 5        // Giới hạn tối đa 5 hàng đợi trong toàn hệ thống
#define MAX_QUEUE_SIZE 256  // Mỗi hàng đợi tối đa 256 byte
// Hồ chứa (プール - ぷーる)
// Tạo sẵn các vùng nhớ tĩnh nằm ở phân vùng BSS của RAM
static MockQueue_t queue_pool[MAX_QUEUES];
static uint8_t queue_data_pool[MAX_QUEUES][MAX_QUEUE_SIZE]; 
static uint32_t queue_count_allocated = 0; // Đếm số Queue đã sử dụng

osMessageQueueId_t osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const void *attr) {
    // Quản lý (管理します - かんりします)
    // 例文: メモリプールを安全(あんぜん)に管理(かんり)します。
    // (Quản lý hồ chứa bộ nhớ một cách an toàn.)
    
    // Nếu tạo quá 5 Queue hoặc dung lượng yêu cầu vượt quá 256 byte -> Báo lỗi ngay!
    if (queue_count_allocated >= MAX_QUEUES || (msg_count * msg_size) > MAX_QUEUE_SIZE) {
        return NULL; 
    }

    // Lấy một Queue ra từ hồ chứa tĩnh, không cần gọi malloc!
    MockQueue_t *q = &queue_pool[queue_count_allocated];
    q->data = &queue_data_pool[queue_count_allocated][0];
    
    q->msg_size = msg_size;
    q->msg_count = msg_count;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    
    // Tăng số lượng (増やします - ふやします)
    // 例文: 使用済(しようず)みのキューの数(かず)を増(ふ)やします。
    // (Tăng số lượng hàng đợi đã sử dụng.)
    queue_count_allocated++;
    
    return (osMessageQueueId_t)q;
}

// ... (Giữ nguyên các hàm osMessageQueuePut và osMessageQueueGet bên dưới) ...

osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout) {
    MockQueue_t *q = (MockQueue_t *)mq_id;
    pthread_mutex_lock(&q->lock);
    
    // Nếu hàng đợi bị đầy (満杯 - まんぱい - đầy)
    if (q->count >= q->msg_count) {
        pthread_mutex_unlock(&q->lock);
        return osErrorResource;
    }

    // Sao chép (コピー - こぴー - sao chép) dữ liệu
    uint32_t offset = q->tail * q->msg_size;
    memcpy((char*)q->data + offset, msg_ptr, q->msg_size);
    
    q->tail = (q->tail + 1) % q->msg_count;
    q->count++;

    // Gửi tín hiệu (信号 - しんごう - tín hiệu) đánh thức luồng đang đợi
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
    return osOK;
}

osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout) {
    MockQueue_t *q = (MockQueue_t *)mq_id;
    pthread_mutex_lock(&q->lock);

    // XỬ LÝ KHI HÀNG ĐỢI TRỐNG
    if (q->count == 0) {
        if (timeout == 0) {
            // Tối ưu hóa Polling: Nhả khóa và trả về lỗi NGAY LẬP TỨC, tuyệt đối không được Block!
            pthread_mutex_unlock(&q->lock);
            return osErrorResource; 
        } else {
            // Nếu có timeout, đành phải chặn luồng chờ đến khi có tín hiệu not_empty
            while (q->count == 0) {
                pthread_cond_wait(&q->not_empty, &q->lock);
            }
        }
    }

    // Nếu có data thì bốc ra như bình thường
    uint32_t offset = q->head * q->msg_size;
    memcpy(msg_ptr, (char*)q->data + offset, q->msg_size);
    
    q->head = (q->head + 1) % q->msg_count;
    q->count--;

    pthread_mutex_unlock(&q->lock);
    return osOK;
}