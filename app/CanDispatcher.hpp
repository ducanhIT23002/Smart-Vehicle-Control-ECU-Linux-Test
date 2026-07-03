#ifndef CAN_DISPATCHER_HPP
#define CAN_DISPATCHER_HPP

#include <stdint.h>

class CanDispatcher {
public:
    CanDispatcher();
    void Init();
    void RunTask();
};

// Cầu nối C-C++
extern "C" {
    void CanDispatcher_Init_Wrapper(void);
    void CanDispatcher_Task_Wrapper(void *argument);
}

#endif // CAN_DISPATCHER_HPP