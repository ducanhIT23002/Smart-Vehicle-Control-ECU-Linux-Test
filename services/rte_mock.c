#include "Rte_Door.h"
#include "Rte_Wiper.h"
#include "Rte_Light.h"
#include <stdio.h>

// ==========================================
// MOCK CHO DOOR (CỬA)
// ==========================================
uint8_t Rte_Read_DoorStatus(void) {
    return 1; // Giả lập trạng thái mặc định (1 = Không có sự thay đổi đột ngột)
}

void Rte_Write_P_DoorLed_Status(uint8_t status) {
    printf("[RTE MOCK] Door LED Status set to: %d\n", status);
}

// ==========================================
// MOCK CHO WIPER (GẠT MƯA)
// ==========================================
uint8_t Rte_Read_WiperSwitch(void) {
    return 1; // Giả lập nút gạt mưa chưa bị bấm
}

void Rte_Write_WiperMode(WiperMode_t mode) {
    printf("[RTE MOCK] Wiper Mode set to: %d\n", mode);
}

// ==========================================
// MOCK CHO LIGHT (ĐÈN)
// ==========================================
uint8_t Rte_Read_DimmerSwitch(void) {
    return 1; // Giả lập nút Dimmer chưa bị bấm
}

EnvLightState_t Rte_Read_EnvLight(void) {
    return ENV_BRIGHT; // Giả lập môi trường đang sáng
}

void Rte_Write_Headlight(uint8_t status) {
    printf("[RTE MOCK] Headlight set to: %d\n", status);
}

void Rte_Call_LightFadeIn(void) {
    printf("[RTE MOCK] Interior Light Fade IN (Dimmer ON)\n");
}

void Rte_Call_LightFadeOut(void) {
    printf("[RTE MOCK] Interior Light Fade OUT (Dimmer OFF)\n");
}