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



















## 🐍 PHASE 2: SOFTWARE-IN-THE-LOOP (SiL) & PYTHON TOOLING
*Nhiệm vụ: Sử dụng Python để tự động hóa và xây dựng môi trường giả lập (đáp ứng yêu cầu Tooling & Scripting của Bosch).*

- [ ] **Day 10: Python UART/CAN Logger & Automation**
    - Viết script Python đọc luồng dữ liệu (log) từ cổng ảo hoặc file text.
    - Parse dữ liệu CAN Frame (ID, Payload) và hiển thị trạng thái xe (Cửa/Đèn) trực quan.
- [ ] **Day 11: Linux Ecosystem & GCC Toolchain**
    - Thiết lập môi trường Ubuntu (WSL/VirtualBox).
    - Thành thạo các lệnh Linux Terminal cơ bản và cài đặt `arm-none-eabi-gcc`.
- [ ] **Day 12: Build Automation (Makefile)**
    - Tự viết `Makefile` quản lý project. Hiểu sâu quy trình Compilation, Linking để tối ưu hóa bộ nhớ.

---

## 🐧 PHASE 3: VIRTUAL ECU & MODERN C++ (Adaptive AUTOSAR Prep)
*Nhiệm vụ: Đưa logic BCM lên chạy đa luồng trên Linux và áp dụng tư duy hướng đối tượng (OOP).*

- [ ] **Day 13: C++ OOP Wrapper cho SWCs**
    - Đóng gói các logic C thuần (Light/Door/Wiper) thành các **Class**.
    - Sử dụng tính chất Đóng gói (Encapsulation) để quản lý tài nguyên hệ thống chuyên nghiệp hơn.
- [ ] **Day 14: POSIX Threads (pthreads) & Synchronization**
    - Chuyển đổi từ CMSIS-RTOS2 sang `pthread` của Linux.
    - Xây dựng Virtual ECU: Các Task chạy song song trên nhân Linux, giao tiếp qua Linux IPC.
- [ ] **Day 15: Mocking Hardware & Unit Test**
    - Viết các hàm "giả lập" phần cứng (Mock) để chạy test logic mà không cần chip.
    - Làm quen với tư duy Test-Driven Development (TDD).

---

## 🚀 PHASE 4: ADVANCED CONNECTIVITY & DEVOPS
*Nhiệm vụ: Kết nối hệ thống và mô phỏng quy trình làm việc tại các tập đoàn lớn.*

- [ ] **Day 16-17: Socket Programming (TCP/IP)**
    - Viết ứng dụng Client-Server để điều khiển BCM từ xa qua mạng LAN (mô phỏng kết nối Cloud/V2X).
- [ ] **Day 18-19: Bash Scripting & CI/CD Pipeline**
    - Viết script tự động hóa toàn bộ quy trình: Build -> Test -> Report.
    - Tìm hiểu cách thức hoạt động của Jenkins/GitHub Actions trong dự án Automotive.
- [ ] **Day 20: Final Review & Bosch Portfolio**
    - Tổng hợp toàn bộ source code, sơ đồ kiến trúc và kết quả test vào GitHub.
    - Chuẩn bị tài liệu kỹ thuật (Design Document) bằng tiếng Anh chuyên ngành.