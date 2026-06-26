#ifndef RTE_WIPER_H
#define RTE_WIPER_H

#include <stdint.h>
#include "Rte_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t Rte_Read_WiperSwitch(void);
void Rte_Write_WiperMode(WiperMode_t mode);

#ifdef __cplusplus
}
#endif

#endif /* RTE_WIPER_H */