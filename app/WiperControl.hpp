#ifndef WIPER_CONTROL_HPP
#define WIPER_CONTROL_HPP

#include <stdint.h>
#include "Rte_Wiper.h" 

class WiperControl {
private:
    // Biến trạng thái đã được đưa vào làm tài sản riêng của Class
    WiperMode_t current_wiper_mode;

public:
    WiperControl();
    void Init();
    void RunTask(); 
};

// Cầu nối C-C++
extern "C" {
    void WiperControl_Init_Wrapper(void);
    void WiperControl_Task_Wrapper(void *argument);
}

#endif // WIPER_CONTROL_HPP