#ifndef RTE_SENSOR_H
#define RTE_SENSOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Hàm ghi Tốc độ xe (Chỉ dành cho CanDispatcher dịch xong và ghi lên)
void Rte_Write_VehicleSpeed(uint8_t speed);

// Hàm đọc Tốc độ xe (Dành cho các Task như Cửa, Đèn lấy ra dùng)
uint8_t Rte_Read_VehicleSpeed(void);



#ifdef __cplusplus
}
#endif
#endif /* RTE_SENSOR_H */