#ifndef RTE_LIGHT_H
#define RTE_LIGHT_H

#include <stdint.h>
#include "Rte_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t Rte_Read_DimmerSwitch(void);
EnvLightState_t Rte_Read_EnvLight(void);
void Rte_Write_Headlight(uint8_t status);
void Rte_Call_LightFadeIn(void);
void Rte_Call_LightFadeOut(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_LIGHT_H */