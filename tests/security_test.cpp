#include <gtest/gtest.h>
#include "SecurityFilter.hpp"

// KỊCH BẢN 1: Tín hiệu khẩn cấp phải LUÔN LUÔN được thông qua (Không cần thẻ)
TEST(SecurityFilterTest, WhitelistAlwaysPasses) {
    SecurityFilter filter;
    
    // Dội bom 20 tin nhắn Túi khí (0x050), vượt quá số thẻ mặc định là 10
    for (int i = 0; i < 20; i++) {
        EXPECT_TRUE(filter.IsFrameAllowed(0x050)); // Phải luôn báo True
    }
}

// KỊCH BẢN 2: Chặn đứng Hacker khi xài hết 10 thẻ mặc định
TEST(SecurityFilterTest, RateLimitBlocksSpam) {
    SecurityFilter filter;
    
    // 10 tin nhắn đầu tiên (ID cửa: 0x100) phải đi qua trót lọt
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(filter.IsFrameAllowed(0x100)); 
    }
    
    // Tin nhắn thứ 11 sẽ bị chặn họng lại ngay lập tức
    EXPECT_FALSE(filter.IsFrameAllowed(0x100)); 
}

// KỊCH BẢN 3: Khả năng hồi máu (Regeneration) của hệ thống
TEST(SecurityFilterTest, TokenRegenerationWorks) {
    SecurityFilter filter;
    
    // Xài sạch 10 thẻ
    for (int i = 0; i < 10; i++) {
        filter.IsFrameAllowed(0x100); 
    }
    EXPECT_FALSE(filter.IsFrameAllowed(0x100)); // Hết bài!
    
    // Giả lập thời gian trôi qua: Gọi hàm UpdateTokens 9 lần (chưa đủ 10ms)
    for (int i = 0; i < 9; i++) {
        filter.UpdateTokens();
    }
    // Vẫn chưa được cấp thẻ mới
    EXPECT_FALSE(filter.IsFrameAllowed(0x100)); 
    
    // Lần lặp thứ 10: Thẻ mới được sinh ra!
    filter.UpdateTokens();
    
    // Gửi lại tin nhắn, lần này phải đi qua được!
    EXPECT_TRUE(filter.IsFrameAllowed(0x100)); 
}