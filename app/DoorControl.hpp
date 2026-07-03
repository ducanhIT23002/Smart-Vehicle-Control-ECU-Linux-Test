#ifndef DOOR_CONTROL_HPP
#define DOOR_CONTROL_HPP

#include <stdint.h>

// Lớp (クラス - くらす)
// 例文: ドア制御(せいぎょ)のクラスを定義(ていぎ)します。
// (Định nghĩa lớp điều khiển cửa.)
class DoorControl {
private:
    uint8_t currentDoorState;

public:
    DoorControl();
    void Init();
    void RunTask(); 
};

// Cầu nối C-C++ cho Hệ điều hành và hàm main
extern "C" {
    void DoorControl_Init_Wrapper(void);
    void DoorControl_Task_Wrapper(void *argument);
}

#endif // DOOR_CONTROL_HPP