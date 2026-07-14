#include <gtest/gtest.h>

// Một bài test ngớ ngẩn để kiểm tra framework
TEST(BasicTest, SystemIsSane) {
    int a = 1;
    int b = 1;
    // Kỳ vọng a + b phải bằng 2
    EXPECT_EQ(a + b, 2);
}