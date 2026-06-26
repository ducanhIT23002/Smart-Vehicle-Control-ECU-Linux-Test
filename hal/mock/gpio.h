#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline void GPIO_SetDir(uint8_t port, uint8_t pin, uint8_t dir) {
    // Mock: Không làm gì cả trên môi trường Linux
}

static inline uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin) {
    return 1; // Mặc định trả về 1 (Joystick chưa bị kích hoạt)
}

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H */