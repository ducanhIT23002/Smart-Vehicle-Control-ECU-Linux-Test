#ifndef INPUT_MONITOR_HPP
#define INPUT_MONITOR_HPP

#include <stdint.h>
#include "Rte_Types.h"

class InputMonitor {
private:
    // Đã gom toàn bộ biến static lơ lửng vào đây để dễ quản lý
    uint8_t last_door;
    uint8_t last_wiper_sw;
    uint8_t last_dimmer_sw;
    EnvLightState_t last_env;
    uint8_t last_joystick;

public:
    InputMonitor();
    void Init();
    void RunTask();
};

extern "C" {
    void InputMonitor_Init_Wrapper(void);
    void InputMonitor_Task_Wrapper(void *argument);
}

#endif // INPUT_MONITOR_HPP