#ifndef LIGHT_CONTROL_HPP
#define LIGHT_CONTROL_HPP

#include <stdint.h>

class LightControl {
private:
    uint8_t is_interior_light_on;

public:
    LightControl();
    void Init();
    void RunTask(); 
};

// Cầu nối C-C++
extern "C" {
    void LightControl_Init_Wrapper(void);
    void LightControl_Task_Wrapper(void *argument);
}

#endif // LIGHT_CONTROL_HPP