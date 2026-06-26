#ifndef RTE_TYPES_H
#define RTE_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================
 * 1. SMART DOOR SYSTEM TYPES
 * ========================================= */
typedef enum {
    DOOR_OPEN = 0,   
    DOOR_CLOSED = 1  
} DoorState_t;


typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} LedState_t;

/* =========================================
 * 2. AUTO WIPER SYSTEM TYPES
 * ========================================= */
typedef enum {
    WIPER_SW_OFF = 1, 
    WIPER_SW_ON = 0  
} WiperSwitchState_t;

typedef enum {
    WIPER_OFF = 0,
    WIPER_INT,        
    WIPER_LOW,        
    WIPER_HIGH        
} WiperMode_t;

/* =========================================
 * 3. AUTO HEADLIGHT SYSTEM TYPES
 * ========================================= */
typedef enum {
    ENV_BRIGHT = 0,
    ENV_DARK = 1
} EnvLightState_t;


/* =========================================
 * 4. SYSTEM EVENT TYPES (FOR RTOS QUEUE)
 * ========================================= */
typedef enum {
    SYS_EVT_NONE = 0,
    SYS_EVT_DOOR_OPENED,
    SYS_EVT_DOOR_CLOSED,
    SYS_EVT_WIPER_BTN_PRESSED,
    SYS_EVT_DIMMER_BTN_PRESSED,
    SYS_EVT_ENV_DARK,
    SYS_EVT_ENV_BRIGHT,
    SYS_EVT_CRASH = 0xFF
} SystemEvent_t;

#ifdef __cplusplus
}
#endif

#endif /* RTE_TYPES_H */