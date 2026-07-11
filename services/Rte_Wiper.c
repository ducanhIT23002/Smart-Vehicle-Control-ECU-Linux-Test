#include "Rte_Wiper.h"

// Biến cờ: 0 là bình thường, 1 là đang chạy > 5s
static uint8_t wiper_long_active = 0;

void Rte_Write_WiperActive_Flag(uint8_t status) {
    wiper_long_active = status;
}

uint8_t Rte_Read_WiperActive_Flag(void) {
    return wiper_long_active;
}