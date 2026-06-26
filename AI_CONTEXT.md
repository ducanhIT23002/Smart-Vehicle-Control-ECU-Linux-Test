# [SYSTEM CONTEXT - SMART BCM SiL PROJECT]

## 🎯 Mục tiêu (目標 - もくひょう - Mục tiêu)
- Xây dựng thành công môi trường Software-in-the-Loop (SiL) hoàn chỉnh cho hệ thống Body Control Module (BCM) ô tô, làm nền tảng mô phỏng tương tác thời gian thực trước khi tích hợp các tính năng nâng cao.

## 💻 Môi trường & Công nghệ (環境 - かんきょう - Môi trường)
- **Hệ điều hành:** Máy ảo Ubuntu Server 24.04 (Kết nối SSH qua Cursor/VS Code).
- **Kiến trúc phần mềm:** Phân lớp nghiêm ngặt theo chuẩn AUTOSAR (Application, Services, HAL/Mock, Simulator).
- **Ngôn ngữ:** C/C++ (Xử lý logic lõi của ECU) và Python 3 (Kịch bản tự động hóa kiểm thử).

---

## 📊 Cấu trúc Thư mục & Trạng thái Hiện tại (現状 - げんじょう - Hiện trạng)

Hệ thống đã thông luồng end-to-end hoàn chỉnh, biên dịch không lỗi và tương tác hai chiều thời gian thực (Closed-loop) thành công qua các file sau:

```text
Smart-BCM-SiL/
├── app/                      # TẦNG ỨNG DỤNG (APPLICATION LAYER)
│   ├── main.c                # Khởi tạo Hệ thống, tạo 6 Luồng (5 SWCs + 1 WdgM), tắt Buffer dòng xuất.
│   ├── CanDispatcher_SWC.c/.h # Bộ điều phối mạng CAN, phân phối Event vào các Message Queue chuẩn.
│   ├── DoorControl_SWC.c/.h   # Tác vụ điều khiển Cửa, kiểm tra Sự kiện, điều khiển LED và gửi CAN phản hồi.
│   ├── InputMonitor_SWC.c/.h  # Giám sát trạng thái nút bấm phần cứng ngoại vi.
│   ├── LightControl_SWC.c/.h  # Tác vụ điều khiển hệ thống Đèn (Headlight, Fade In/Out).
│   └── WiperControl_SWC.c/.h  # Tác vụ điều khiển Gạt mưa (Chế độ OFF, INT).
├── services/                 # TẦNG DỊCH VỤ NỀN TẢNG (SERVICES LAYER)
│   ├── cmsis_os2.h           # Khai báo chuẩn API RTOS2, chuyển hướng macro cấu trúc __NO_RETURN của ARM GCC.
│   ├── cmsis_os2_mock.c      # Giả lập Hệ điều hành: Ánh xạ Thread, Mutex, Message Queue bằng thư viện <pthread.h> Linux.
│   ├── Rte_Types.h           # Định nghĩa tập trung toàn bộ Kiểu dữ liệu, Event ID, Task ID theo chuẩn AUTOSAR.
│   ├── Rte_Door.h / Rte_Wiper.h / Rte_Light.h / Rte_Sensor.h # Khai báo API giao tiếp của tầng RTE.
│   ├── rte_mock.c            # Triển khai phần thân (body) hàm RTE, in log định dạng kiểm chứng dữ liệu.
│   ├── wdg_manager.h         # Khai báo trạm giám sát Checkpoint của Watchdog Manager.
│   └── wdg_manager_mock.c    # Tạo luồng Watchdog chạy ngầm giám sát an toàn hệ thống.
├── hal/mock/                 # TẦNG PHẦN CỨNG GIẢ LẬP (HAL MOCK LAYER)
│   ├── uart.h / uart_mock.c  # Chuyển hướng dữ liệu truyền UART vật lý thành printf xuất ra màn hình PC.
│   ├── can_if.h / can_if_mock.c # Giả lập Bộ điều khiển CAN: fgets đọc chuỗi lệnh từ Python, sscanf bóc tách gói tin.
│   └── gpio.h                # Khai báo tĩnh các hàm static inline đọc/ghi chân tín hiệu logic số.
└── simulator/                # TẦNG MÔ PHỎNG & KIỂM THỬ (SIMULATOR LAYER)
    └── auto_test.py          # Script Python đóng vai trò Chúa tể môi trường, nạp dữ liệu phi đối xứng (fcntl O_NONBLOCK), tiêm mã sự kiện thời gian thực.


## 🚀 PROMPT BLUEPRINT CHO TASK TIẾP THEO (DÙNG CHO PHASE 4)
*(Sau khi phân tích xong mã nguồn, người dùng sẽ copy đoạn dưới đây gửi cho AI để làm tiếp)*

"Chào bạn, tôi và bạn đã hoàn thành việc phân tích chuyên sâu dòng code của dự án Smart BCM SiL. Bây giờ, dựa trên file AI_CONTEXT.MD đã lưu, chúng ta sẽ chính thức bước sang PHASE 4: NÂNG CẤP HỆ THỐNG THEO TIÊU CHUẨN TIER-1 AUTOMOTIVE (BOSCH STANDARD). 

Mục tiêu của tôi là biến project này thành một sản phẩm Enterprise-grade. Hãy giúp tôi thiết kế và nâng cấp kiến trúc với 4 trụ cột sau:

1. **Chuẩn hóa MISRA C & Kiến trúc (Quality & Compliance):** - Tái cấu trúc mã nguồn để tuân thủ các quy tắc MISRA C cơ bản (loại bỏ cấp phát bộ nhớ động rủi ro, quản lý kiểu dữ liệu chặt chẽ).
   - Thêm comment chuẩn Doxygen cho toàn bộ các file và hàm.
2. **Tích hợp Chẩn đoán lỗi DEM & UDS (Diagnostics):**
   - Xây dựng module Diagnostic Event Manager (DEM) để lưu trữ các mã lỗi DTC (Diagnostic Trouble Code).
   - Python sẽ đóng vai trò là máy Tester, gửi các gói tin chuẩn UDS qua CAN để đọc lỗi (Read DTC) và xóa lỗi (Clear DTC) của hệ thống.
3. **Giả lập bộ nhớ NvM (Non-Volatile Memory):**
   - Tạo module mô phỏng EEPROM (đọc/ghi file nội bộ trên Linux) để lưu trữ các tham số quan trọng (VD: Số lần đóng/mở cửa, trạng thái lỗi) nhằm đảm bảo dữ liệu sống sót khi ECU bị khởi động lại.
4. **Bộ khung Automation Test Report:**
   - Nâng cấp file `auto_test.py` thành một framework kiểm thử chuyên nghiệp: Tự động chạy hàng chục Test Cases, bao gồm cả Fault Injection (tiêm mã lỗi), sau đó tự động sinh ra một file báo cáo Test Report (TXT hoặc HTML).

Hãy tiếp tục duy trì phong cách giải thích tư duy kỹ thuật rõ ràng, step-by-step, và đặc biệt KHÔNG QUÊN chèn các từ vựng tiếng Nhật chuyên ngành IT (Kanji - Hiragana - Nghĩa + chia động từ V-masu) trong lúc hướng dẫn tôi nhé.

Bạn đã sẵn sàng hóa thân thành Senior Software Engineer để dẫn dắt tôi thực hiện các tính năng này chưa?"