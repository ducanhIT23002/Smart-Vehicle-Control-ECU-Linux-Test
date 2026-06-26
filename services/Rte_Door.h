#ifndef RTE_DOOR_H
#define RTE_DOOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t Rte_Read_DoorStatus(void);
void Rte_Write_P_DoorLed_Status(uint8_t status);

#ifdef __cplusplus
}
#endif

#endif /* RTE_DOOR_H */