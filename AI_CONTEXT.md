# [SYSTEM CONTEXT - SMART BCM SiL PROJECT]

## 🎯 Mục tiêu (目標 - もくひょう - Mục tiêu)
- Xây dựng thành công môi trường Software-in-the-Loop (SiL) đa luồng cho hệ thống Body Control Module (BCM), nâng cấp kiến trúc từ C thuần (Hướng thủ tục) lên C++ (Hướng đối tượng) theo chuẩn Adaptive AUTOSAR.
- Hệ thống phải đạt tiêu chuẩn công nghiệp: Có chẩn đoán lỗi (UDS/NvM), có An toàn chức năng (Functional Safety) và Bảo mật (Cybersecurity).

## 💻 Môi trường & Công nghệ (環境 - かんきょう - Môi trường)
- **Hệ điều hành:** Máy ảo Ubuntu Server 24.04 (Kết nối SSH qua VS Code).
- **Trình biên dịch:** Lai giữa `gcc` và `g++`.
- **Kiến trúc phần mềm:** Phân lớp AUTOSAR, áp dụng triệt để OOP.
- **Ngôn ngữ:** Modern C/C++ và Python 3.

---

## 📊 Cấu trúc Thư mục & Trạng thái Hiện tại (現状 - げんじょう - Hiện trạng)

Hệ thống đã hoàn thiện toàn bộ kiến trúc lõi, đặc biệt là Layer An toàn & Bảo mật (Safety & Security):

```text
Smart-BCM-SiL/
├── app/                      # TẦNG ỨNG DỤNG C++ (APPLICATION LAYER)
│   ├── CanDispatcher.cpp/.hpp # Đã tích hợp UDS (0x7E0) và Thuật toán Token Bucket (Rate Limiting) chặn tấn công DoS.
│   ├── NvM.cpp/.hpp          # Giả lập EEPROM. Xử lý logic RAM Mirroring.
│   ├── LightControl.cpp      # Có bẫy mã độc (Deadlock) để test Functional Safety.
│   └── (Door/Wiper/Input)    # Các Task độc lập, sử dụng kiến trúc Polling (osDelay) tối ưu CPU.
├── services/                 # TẦNG DỊCH VỤ NỀN TẢNG (SERVICES LAYER)
│   ├── wdg_manager_mock.c    # Watchdog Timer: Giám sát sinh tồn. Tích hợp cơ chế Fail-Safe (Bật đèn pha cứu hộ) chuẩn ISO 26262.
│   └── cmsis_os2_mock.c      # OS Kernel: Đã vá lỗi Blocking I/O, hỗ trợ timeout = 0 (Polling).
├── hal/mock/                 # TẦNG PHẦN CỨNG GIẢ LẬP (HAL MOCK LAYER)
│   ├── can_if_mock.c         # Chứa TCP/IP Socket (Server) cổng 8080.
│   └── uart_mock.c           # Giao tiếp Log ra Terminal.
└── simulator/                # TẦNG MÔ PHỎNG & TOOLING (PYTHON LAYER)
    ├── dashboard.py          # Đã nâng cấp thành Máy quét UDS & Tool giả lập tấn công (SPAM/CRASH).
    └── eeprom_mock.dat       # File nhị phân lưu trữ mã lỗi vĩnh cửu.


🚀 PROMPT BLUEPRINT CHO TASK TIẾP THEO (DÙNG CHO PHASE 7)
(Sử dụng đoạn prompt này để thiết lập ngữ cảnh cho AI khi bắt đầu phiên làm việc mới)

"Chào bạn, tôi và bạn đã hoàn thành xuất sắc PHASE 6 (Functional Safety & Cybersecurity). Hệ thống BCM nay đã có khiên chống DoS (Token Bucket) và phao cứu sinh Fail-Safe chuẩn ISO 26262. Bây giờ, dựa trên file AI_CONTEXT.MD đã lưu, chúng ta sẽ chính thức bước sang PHASE 7: AUTOMATION TESTING & SYSTEM PACKAGING (Tự động hóa kiểm thử & Đóng gói hệ thống).

Mục tiêu của tôi là biến khối kiến trúc này thành một sản phẩm hoàn thiện, sẵn sàng đem đi bảo vệ Capstone Project và đưa vào Portfolio phỏng vấn. Hãy giúp tôi triển khai 2 nhiệm vụ cuối cùng:

Hoàn thiện Framework Auto-Test (Tự động hóa kiểm thử):

Viết logic cho file auto_test.py để nó tự động bật BCM C++, kết nối Socket và chạy liên hoàn 3 kịch bản: Kịch bản Chuẩn (Mở cửa/Gạt mưa) -> Kịch bản Tấn Công (SPAM 500) -> Kịch bản An Toàn (CRASH LIGHT).

Python phải tự động đọc luồng Log (stdout) của C++, chấm điểm PASS/FAIL cho từng kịch bản và xuất ra một file báo cáo Test_Report.txt thật chuyên nghiệp.

Clean Code & Đóng gói (Refactoring & Packaging):

Rà soát lại cấu trúc Makefile để build tối ưu nhất.

Hướng dẫn tôi cấu trúc lại các thư mục để code trở nên gọn gàng (整理します - せいりします - Sắp xếp/Dọn dẹp) theo chuẩn công nghiệp trước khi đẩy lên Git.

Vẫn giữ nguyên phong cách Senior: Hướng dẫn trực diện, giải thích rõ luồng dữ liệu (Data Flow) giữa Python và C++ trong lúc Auto-Test. Nhớ chèn các từ vựng tiếng Nhật chuyên ngành IT (Kanji - Hiragana - Nghĩa) trong lúc giải thích nhé.

Bạn đã sẵn sàng cùng tôi chốt sổ dự án đồ sộ này ở Phase 7 chưa?

PHASE 7: AUTOMATION TESTING & PACKAGING
├── app/
│   └── LightControl.cpp    # FIX BUG: Thêm osDelay(1000) vào bẫy Deadlock để chống Trình biên dịch xóa code (Compiler Optimization).
├── simulator/
│   ├── auto_test.py        # CORE: Kịch bản Python tự động boot hệ điều hành C++, nã đạn qua TCP/IP, quét Log và chấm điểm.
│   └── Test_Report.txt     # OUTPUT: File báo cáo nghiệm thu do Python tự động sinh ra.
├── README.md               # NEW: Tài liệu đặc tả dự án chuẩn Enterprise (Viết bằng tiếng Anh).
└── ROADMAP.md              # UPDATE: Cập nhật lộ trình phát triển Full-Stack Automotive.

Chào bạn, tôi đã xem xét xong cấu trúc của Phase 7. Bây giờ chúng ta hãy đi từ từ:

Đầu tiên, hãy giải phẫu chi tiết luồng code của file simulator/auto_test.py và cách chúng ta fix lỗi while(1) trong app/LightControl.cpp dưới góc nhìn của một Senior C++/Python.

Sau khi tôi hiểu code Phase 7, hãy giúp tôi Review lại toàn bộ Tech Stack, Protocol và Workflow của toàn bộ dự án (Từ Phase 1 đến 7) giống như một buổi Mock Interview. Tôi cần những 'văn mẫu' sắc bén để chứng minh năng lực System Architecture khi đi phỏng vấn vị trí Embedded Automotive tại Bosch.
Đi từng bước một nhé, bắt đầu bằng việc giải phẫu code.