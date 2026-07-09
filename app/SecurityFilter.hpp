#ifndef SECURITY_FILTER_HPP
#define SECURITY_FILTER_HPP

#include <stdint.h>

class SecurityFilter {
private:
    int32_t tokens;              // Số thẻ bài hiện tại trong xô
    uint32_t loop_counter;       // Bộ đếm thời gian để hồi phục thẻ bài

public:
    SecurityFilter();            // Khởi tạo
    bool IsFrameAllowed(uint32_t can_id); // Hàm kiểm tra gói tin
    void UpdateTokens();         // Hàm hồi phục thẻ bài (đặt trong vòng lặp)
};

#endif // SECURITY_FILTER_HPP