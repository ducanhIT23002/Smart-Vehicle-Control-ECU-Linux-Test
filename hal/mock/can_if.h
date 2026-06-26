#ifndef CAN_IF_H
#define CAN_IF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Cấu trúc (構造体 - こうぞうたい - cấu trúc) bản tin CAN
typedef struct {
    uint32_t id;       // ID của bản tin CAN
    uint8_t dlc;       // Độ dài dữ liệu (Data Length Code)
    uint8_t data[8];   // Mảng chứa dữ liệu tối đa 8 byte
} CanMessage_t;

void CanIf_Init(void);
uint8_t CanIf_Transmit(const CanMessage_t *msg);
uint8_t CanIf_Receive(CanMessage_t *msg);

#ifdef __cplusplus
}
#endif

#endif /* CAN_IF_H */