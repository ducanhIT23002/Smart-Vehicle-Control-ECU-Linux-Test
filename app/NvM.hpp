#ifndef NVM_HPP
#define NVM_HPP

#include <stdint.h>

// 1. Khai báo Struct chứa dữ liệu cần nhớ
typedef struct {
    uint16_t dtc_queue_full_count; // Đếm số lần bị tấn công tràn Queue
    uint8_t  last_door_state;      // Nhớ trạng thái cửa cuối cùng (chưa dùng tới)
} NvM_DataBlock_t;

class NvM {
private:
    NvM_DataBlock_t ram_mirror; // Phân vùng RAM (đọc/ghi siêu tốc)
    const char* file_path;      // Đường dẫn file giả lập EEPROM

public:
    NvM();
    void Init();                // Chạy lúc boot: Kéo data từ EEPROM lên RAM
    void WriteAll();            // Xả data từ RAM xuống EEPROM
    
    // API cho các Module khác xài
    void Inc_QueueFullError();
    uint16_t Get_QueueFullError();
    void Clear_QueueFullError();
};

extern NvM myNvM; // Instance duy nhất (Singleton pattern)

#endif