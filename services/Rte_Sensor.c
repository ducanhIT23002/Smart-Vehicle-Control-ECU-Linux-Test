#include "Rte_Sensor.h"

// Biến tĩnh (static) lưu trữ tốc độ xe. 
// Dùng static để bảo mật, không cho file khác chọc thẳng vào biến này mà phải qua hàm.
static uint8_t current_vehicle_speed = 0;

void Rte_Write_VehicleSpeed(uint8_t speed) {
    current_vehicle_speed = speed;
}

uint8_t Rte_Read_VehicleSpeed(void) {
    return current_vehicle_speed;
}