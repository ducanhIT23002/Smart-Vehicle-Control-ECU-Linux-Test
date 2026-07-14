# Level 1: Định nghĩa cơ bản
cmake_minimum_required(VERSION 3.24)
project(Smart_BCM_SiL C CXX)

# Level 2: Chỉ đường cho CMake biết file Header (.h, .hpp) nằm ở đâu
include_directories(
    app
    services
    hal/mock
)

# Level 3: Tự động gom TẤT CẢ file .cpp và .c vào một mảng tên là SOURCES
file(GLOB_RECURSE SOURCES
    "app/*.cpp"
    "services/*.c"
    "hal/mock/*.c"
)

# Level 4: Ra lệnh build
add_executable(bcm_sil_app ${SOURCES})
target_link_libraries(bcm_sil_app pthread)

# ==========================================
# CẤU HÌNH KIỂM THỬ (GOOGLE TEST) - MODERN CMAKE
# ==========================================
enable_testing()

# 1. Nhúng tính năng FetchContent của CMake
include(FetchContent)

# 2. Khai báo nguồn tải GoogleTest (Bản ổn định 1.14.0 từ GitHub)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# 3. Ra lệnh tải và chuẩn bị sẵn sàng thư viện
FetchContent_MakeAvailable(googletest)

# 4. Tạo chương trình test như cũ
# Nạp file Test và những file ruột của hệ thống cần thiết cho bài Test
add_executable(run_tests 
    tests/security_test.cpp 
    app/SecurityFilter.cpp 
    hal/mock/uart_mock.c
)
# 5. Gắn thư viện gtest_main (nó viết thường, do FetchContent quản lý) và pthread
target_link_libraries(run_tests gtest_main pthread)

# 6. Đăng ký chương trình
add_test(NAME AllTests COMMAND run_tests)