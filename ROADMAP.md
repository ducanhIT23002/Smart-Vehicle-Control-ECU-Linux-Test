# 🗺️ Project Roadmap: Smart BCM (Bosch-Targeted SiL Edition)

**Mục tiêu:** Chuyển đổi từ lập trình vi điều khiển cơ bản sang quy trình phát triển phần mềm ô tô chuyên nghiệp (Automotive Software Engineering) hướng tới tiêu chuẩn của **Bosch (BGSW)**. Trọng tâm: C/C++, Python Tooling, Linux System và Software-in-the-Loop (SiL).

---

## ✅ PHASE 1: HARDWARE FOUNDATION (COMPLETED)
*Nhiệm vụ: Làm chủ phần cứng, RTOS và các giao thức ngoại vi cơ bản trên LPC1768.*

- [x] **Day 1-4:** Khởi tạo Layered Architecture, UART Debugging, PWM Dimming và ADC Sensor.
- [x] **Day 5-6:** - **CAN Bus:** Cấu hình Controller, chạy Loopback Mode thành công.
    - **Safety:** Tích hợp Watchdog Timer (WDT) giám sát Alive Supervision.
- [x] **Day 7-9:** - **Architecture:** Tối ưu hóa `main.c` bằng `EcuM` (ECU State Manager).
    - **Sync:** Xử lý Race Condition bằng **Mutex** và truyền tin bằng **Queue**.

**🛑 TRẠNG THÁI: Đã bàn giao phần cứng. Chuyển hoàn toàn sang môi trường mô phỏng trên Laptop.**


## 🧠 PHASE 1.5: C CORE & MEMORY MASTERY (THE BOSCH PREREQUISITE)
*Nhiệm vụ: Nắm vững quản lý bộ nhớ ở mức thấp (Low-level Memory Management) - Kỹ năng sống còn của Kỹ sư Embedded.*

- [ ] **Hiểu rõ Kiến trúc Bộ nhớ (Memory Layout):** Phân biệt vùng nhớ Code, BSS, Data, Heap, Stack trong RAM.
- [ ] **Làm chủ Con trỏ (Pointers):** Con trỏ hàm, ép kiểu con trỏ (Type Casting), con trỏ `void*`.
- [ ] **Cấp phát bộ nhớ động (Dynamic Allocation):** Cơ chế hoạt động của `malloc()`, `calloc()`, `free()`.
- [ ] **Lỗi bộ nhớ (Memory Pitfalls):** Nhận diện Memory Leak (rò rỉ), Dangling Pointer (con trỏ lơ lửng), Stack Overflow (tràn ngăn xếp).

---
## 🐍 PHASE 2: SOFTWARE-IN-THE-LOOP (SiL) & PYTHON TOOLING
*Nhiệm vụ: Chuyển BCM sang môi trường mô phỏng trên PC và dùng Python để hỗ trợ test, log và phân tích dữ liệu.*

- [ ] **Day 10: Build BCM Simulator trên PC**
  - Tách phần logic BCM ra khỏi phần phụ thuộc phần cứng.
  - Tạo lớp mock cho input/output: Door, Light, Wiper, Ignition, Sensor.
  - Chạy được BCM logic mà không cần board thật.

- [ ] **Day 11: Python Log Reader & Scenario Runner**
  - Viết script Python đọc log từ file text/CSV/JSON.
  - Tạo scenario mô phỏng các tình huống như:
    - mở cửa
    - bật đèn
    - gạt mưa
    - lỗi tín hiệu
  - Python sẽ đóng vai trò là công cụ test tự động cho BCM.

- [ ] **Day 12: Python CAN/UART Visualizer**
  - Parse frame hoặc event log và hiển thị trạng thái hệ thống.
  - Ví dụ:
    - Door = OPEN/CLOSE
    - Light = ON/OFF
    - Wiper = LOW/HIGH
  - Mục tiêu là nhìn được luồng hoạt động thay vì chỉ xem text log.

- [ ] **Day 13: Linux Environment & Toolchain**
  - Thiết lập Ubuntu/WSL.
  - Cài `gcc`, `g++`, `make`, `cmake`, `python3`, `git`.
  - Làm quen với terminal, compile, run, debug bằng command line.

- [ ] **Day 14: Build Automation**
  - Viết `Makefile` cho project BCM.
  - Tự động hóa các bước:
    - build
    - run
    - test
    - clean
  - Hiểu rõ compilation, linking, include path, object file.

---

## 🐧 PHASE 3: VIRTUAL ECU & SYSTEM DESIGN
*Nhiệm vụ: Biến BCM thành một Virtual ECU có thể chạy như phần mềm hệ thống, không phụ thuộc chip thật.*

- [ ] **Day 15: Tổ chức lại kiến trúc module**
  - Tách rõ:
    - Application Layer
    - Service Layer
    - Communication Layer
    - Platform/Driver Abstraction
    - Simulator Layer
  - Mỗi module chỉ làm đúng nhiệm vụ của nó.

- [ ] **Day 16: C++ Wrapper cho các chức năng BCM**
  - Đóng gói các logic như Light, Door, Wiper thành class/module rõ ràng.
  - Dùng OOP để quản lý trạng thái và tài nguyên tốt hơn.
  - Không làm OOP quá phức tạp, chỉ dùng khi nó giúp code sạch hơn.

- [ ] **Day 17: Virtual ECU chạy trên Linux**
  - Chuyển BCM logic sang chạy như một process trên Linux.
  - Dùng `pthread` hoặc event loop để mô phỏng task song song.
  - Giao tiếp giữa các module bằng queue, pipe, hoặc socket nội bộ.

- [ ] **Day 18: Mocking Hardware**
  - Viết mock cho các hàm đọc/ghi phần cứng.
  - Ví dụ:
    - `read_door_sensor()`
    - `set_headlamp()`
    - `set_wiper_speed()`
  - Mục tiêu: code logic không phụ thuộc chip thật.

- [ ] **Day 19: Unit Test cho State Machine**
  - Test các case quan trọng:
    - input hợp lệ
    - input lỗi
    - chuyển trạng thái đúng
    - timeout
    - recovery
  - Tập trung test logic trước, không cần test phần cứng.

---

🚀 PHASE 4: ADVANCED CONNECTIVITY & DIAGNOSTICS (Đang thực hiện)
Nhiệm vụ: Cô lập lỗi hệ thống, kiểm thử ép tải và đưa ECU kết nối ra thế giới bên ngoài (Connected Car).

[x] Day 20: Fault Injection & Stress Testing (Đã hoàn thành)

Bơm dữ liệu rác (Noise) và ép tải nặng (Spam 10,000 lệnh) qua Pipe.

Cấu trúc C++ nhận diện tràn Queue và xuất log cảnh báo (DTC - Diagnostic Trouble Code).

[ ] Day 21: TCP/IP Socket Programming (V2X Simulation)

Tháo bỏ Pipe stdin/stdout, thay bằng giao tiếp mạng TCP/IP Sockets trên Linux.

BCM C++ đóng vai trò là Server lắng nghe trên một Port. Python đóng vai trò là Client kết nối từ xa.

Mô phỏng kiến trúc xe hơi kết nối mạng (通信 - つうしん - Giao tiếp/Truyền thông).

[ ] Day 22: Centralized Logging System

Ghi log (記録します - きろくします - Ghi lại) với định dạng chuẩn: [Timestamp] [Module] [Event] [Payload].

Lưu log ra file .log xoay vòng (Log Rotation) để tránh đầy ổ cứng bộ nhớ ảo.

🛡️ PHASE 5: UDS DIAGNOSTICS & NON-VOLATILE MEMORY (Chuẩn Công nghiệp)
Nhiệm vụ: Áp dụng chuẩn chẩn đoán quốc tế ISO 14229 (UDS) và lưu trữ dữ liệu vĩnh viễn (NvM).

[ ] Day 23: NvM (Non-Volatile Memory) Mocking

Giả lập EEPROM bằng cách đọc/ghi file .dat hoặc .json trên Linux.

Lưu lại trạng thái xe (Cửa đang mở/đóng, Số lần gạt mưa) để khi BCM sập nguồn (Crash) và khởi động lại, nó vẫn nhớ trạng thái cũ.

[ ] Day 24: UDS (Unified Diagnostic Services) Protocol

Triển khai chuẩn ISO 14229 cơ bản.

Lập trình Python gửi các Service ID chuẩn: 0x19 (Read DTC - Đọc lỗi), 0x14 (Clear DTC - Xóa lỗi), 0x2E (Write Data - Ghi dữ liệu cấu hình).

BCM phân tích gói tin UDS và phản hồi (応答します - おうとうします - Phản hồi) đúng chuẩn.

🔒 PHASE 6: CYBERSECURITY & FUNCTIONAL SAFETY
Nhiệm vụ: Chống Hacker tấn công và đảm bảo an toàn tính mạng người dùng (ISO 21434 & ISO 26262).

[ ] Day 25: DoS Protection (Token Bucket Algorithm)

Viết thuật toán giới hạn tốc độ (Rate Limiting) ngay tại cổng CAN Dispatcher.

Ngăn chặn hệ thống bị nghẽn cổ chai khi Hacker cố tình Spam mạng CAN.

[ ] Day 26: SecOC (Secure Onboard Communication)

Thêm mã xác thực (MAC - Message Authentication Code) vào cuối gói tin CAN/TCP.

BCM chỉ xử lý lệnh từ Python nếu chữ ký điện tử hợp lệ (Xác thực - 認証します - にんしょうします).

[ ] Day 27: Phục hồi sau thảm họa (Error Recovery)

Tối ưu Watchdog Manager: Khi phát hiện 1 Task bị treo (Deadlock), không chỉ reset cả hệ thống mà phải đưa các cơ cấu chấp hành (Actuators) về "Safe State" (Ví dụ: Treo đèn thì mặc định phải bật sáng, không được tắt tối thui).

🏭 PHASE 7: ENTERPRISE CI/CD & MISRA COMPLIANCE
Nhiệm vụ: Đóng gói dự án theo chuẩn khắt khe của hệ thống phần mềm nhúng doanh nghiệp.

[ ] Day 28: MISRA C++ & Static Code Analysis

Chạy các công cụ phân tích tĩnh (Static Analyzer) như Cppcheck.

Loại bỏ hoàn toàn các rủi ro tràn bộ đệm, ép kiểu sai, đảm bảo không có cảnh báo nào khi biên dịch với cờ -Wall -Wextra -Werror.

[ ] Day 29: Automated Testing Framework

Nâng cấp auto_test.py thành một Unit/Integration Test Framework chuyên nghiệp bằng pytest.

Sinh báo cáo kiểm thử định dạng HTML (Test Report) rõ ràng.

[ ] Day 30: CI/CD Pipeline & Final Portfolio

Tích hợp GitHub Actions: Cứ mỗi lần push code, server GitHub sẽ tự động make build, tự chạy pytest, tự động báo PASS/FAIL.

Hoàn thiện tài liệu kiến trúc (State Machine, Sequence Diagram) lên README.




# 🗺️ ROADMAP: SMART BCM FULL-STACK AUTOMOTIVE
*(Aligned with Bosch IVI & Base Software Requirements)*

## ✅ PHASE 1-7: CORE MIDDLEWARE (Software-in-the-Loop) - [COMPLETED]
- **Kiến trúc (Architecture):** Chuyển đổi C sang C++ (OOP), phân lớp chuẩn AUTOSAR (App, RTE, Services, HAL).
- **Hệ điều hành (OS):** Giả lập RTOS bằng POSIX Threads (Linux/Ubuntu), Quản lý Mutex, Queue.
- **Mạng (Network):** Giả lập CAN Bus qua giao thức TCP/IP Socket (C++ Server, Python Client).
- **Chẩn đoán (Diagnostics):** Tích hợp chuẩn UDS (ISO 14229) đọc/xóa lỗi (DTC), lưu trữ EEPROM (NvM).
- **An toàn (Safety - ISO 26262):** Watchdog Timer đếm ngược, kích hoạt Fail-Safe (Bật đèn khẩn cấp) khi Deadlock.
- **Bảo mật (Cybersecurity):** Thuật toán Token Bucket chống tấn công DoS/Spam tràn RAM.
- **Kiểm thử (Testing):** Kịch bản CI/CD Python tự động hóa kiểm thử (Auto-Test).

## 🚀 PHASE 8: HMI & CAR MULTIMEDIA (Web-based Dashboard) - [NEXT]
*Targeting JD: Knowledge of Web (React/Python) / Linux-based system.*
- Xóa bỏ giao diện Terminal đen ngòm của Python.
- Xây dựng **Middleware Gateway (FastAPI/Python)** giao tiếp WebSocket với Front-end.
- Xây dựng **React.js HMI**: Giao diện màn hình xe ô tô trực quan (Có vô lăng, nút bấm, đèn báo).
- **Data Flow:** Click Web (React) -> WebSocket (Python) -> TCP/IP Socket -> C++ BCM (Linux).

## 🔌 PHASE 9: HARDWARE-IN-THE-LOOP (HiL) - [FUTURE]
*Targeting JD: MCU (STM32/LPC), SPI/I2C/UART, Bootloader, RTOS.*
- Bóc tách Module `InputMonitor` và `LightControl` đưa xuống vi điều khiển LPC1768/STM32 thật.
- Cấu hình FreeRTOS trên vi điều khiển.
- Viết Driver giao tiếp **SPI hoặc UART** để vi điều khiển nói chuyện với "Bộ não" Linux C++ thay vì dùng Mock.
- Viết Driver I2C để lưu lỗi UDS vào chip EEPROM vật lý thay vì file `.dat`.

## 🌐 PHASE 10: ADVANCED AUTOMOTIVE NETWORKING
*Targeting JD: SOME/IP, Ethernet, D-Bus.*
- Nâng cấp giao tiếp nội bộ Linux (IPC) bằng D-Bus.
- Thay thế TCP thô bằng SOME/IP (Scalable service-Oriented MiddlewarE over IP) - Tiêu chuẩn cao cấp nhất của xe hơi hiện đại.