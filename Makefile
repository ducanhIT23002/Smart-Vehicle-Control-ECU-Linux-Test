# Trình biên dịch (コンパイラ - こんぱいら)
# 例文: C++のコンパイラを使(つか)います。
# (Sử dụng trình biên dịch C++.)
CC = gcc
CXX = g++

# Cờ cấu hình dùng chung cho cả C và C++
CFLAGS = -Wall -pthread -I./app -I./services -I./hal/mock -I./simulator 
CXXFLAGS = $(CFLAGS)

# ============================================================================
# 1. TÌM KIẾM TỰ ĐỘNG
# ============================================================================
# Tách riêng biệt nguyên liệu C và nguyên liệu C++
C_SRC = $(wildcard services/*.c) $(wildcard hal/mock/*.c)
CXX_SRC = $(wildcard app/*.cpp)

# Đổi đuôi tương ứng (.c thành .o) và (.cpp thành .o)
OBJ = $(C_SRC:.c=.o) $(CXX_SRC:.cpp=.o)
EXEC = bcm_sil_app

# ============================================================================
# 2. CÔNG THỨC CHÍNH (TẠO FILE CHẠY)
# ============================================================================
all: $(EXEC)

# Liên kết (リンクします - りんくします)
# LƯU Ý TỬ THẦN: Bắt buộc phải dùng $(CXX) tức là g++ ở bước gộp cuối cùng 
# vì hàm main() của chúng ta giờ đã là C++.
$(EXEC): $(OBJ)
	$(CXX) -pthread -o $@ $^

# ============================================================================
# 3. CÔNG THỨC SƠ CHẾ (BIÊN DỊCH)
# ============================================================================
# Đầu bếp g++ chuyên xử lý thịt (file .cpp)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Đầu bếp gcc chuyên xử lý rau (file .c)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================================
# 4. DỌN DẸP
# ============================================================================
clean:
	rm -f $(OBJ) $(EXEC) *_ARM.o

# ============================================================================
# KIỂM TRA TÀI NGUYÊN BỘ NHỚ CHO CHIP ARM
# ============================================================================
check_ram:
	arm-none-eabi-g++ -c app/CanDispatcher.cpp -o CanDispatcher_ARM.o -I./services -I./hal/mock -I./app
	arm-none-eabi-g++ -c app/DoorControl.cpp -o DoorControl_ARM.o -I./services -I./hal/mock -I./app
	arm-none-eabi-g++ -c app/LightControl.cpp -o LightControl_ARM.o -I./services -I./hal/mock -I./app
	arm-none-eabi-g++ -c app/WiperControl.cpp -o WiperControl_ARM.o -I./services -I./hal/mock -I./app
	
	@echo "--- KÍCH THƯỚC BỘ NHỚ TRÊN KIẾN TRÚC ARM 32-BIT ---"
	arm-none-eabi-size *_ARM.o
	rm -f *_ARM.o