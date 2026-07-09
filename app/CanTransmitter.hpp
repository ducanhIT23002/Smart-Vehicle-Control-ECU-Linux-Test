#ifndef CAN_TRANSMITTER_HPP
#define CAN_TRANSMITTER_HPP

#include <stdint.h>

// Định nghĩa Cấu trúc Bức thư nội bộ (Internal Event)
// Được dùng chung bởi InputMonitor (Người gửi) và CanTransmitter (Người nhận)
typedef struct {
    uint32_t event_id;
    uint8_t payload;
} InternalEvent_t;

// Cầu nối để Hệ điều hành RTOS (viết bằng C) có thể gọi được luồng này
#ifdef __cplusplus
extern "C" {
#endif

void CanTransmitter_Task_Wrapper(void *argument);

#ifdef __cplusplus
}
#endif

#endif // CAN_TRANSMITTER_HPP