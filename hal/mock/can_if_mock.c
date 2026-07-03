#include "can_if.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
// 3 Thư viện chuyên dụng cho Socket Mạng trên Linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//<sys/socket.h>: Chứa các hàm core socket(), bind(), listen(), accept(), recv().
//<netinet/in.h> & <arpa/inet.h>: Chứa các struct sockaddr_in và các macro ép kiểu IP/Port.




#define PORT 8080 // Cổng giao tiếp mạng (Mở Port 8080)

// Biến lưu trữ ID của Hộp thư
static int server_fd = -1;
static int client_socket = -1;

// ============================================================================
// KHỞI TẠO PHẦN CỨNG CAN (Bây giờ là Mở Server Mạng)
// ============================================================================
void CanIf_Init(void) {
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 1. Tạo Socket (ソケットを作成します - そけっとをさくせいします)
    //SOCK_STREAM " TCP | SOCK_DGRAM " UDP | AF_INET : ipv4
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        UART0_SendString("🔥 [CAN MOCK] Socket creation failed!\r\n");
        exit(EXIT_FAILURE);
    }

    // Cấu hình chống kẹt Port (Address Reuse)
    //(再利用します - さいりようします - Tái sử dụng)
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Chấp nhận kết nối từ mọi IP
    address.sin_port = htons(PORT); //đổi Port số nguyên bình thường sang hệ Big-Endian (chuẩn bắt buộc của mạng lưới).

    // 2. Khóa Port (バインドします - ばいんどします)
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        UART0_SendString("🔥 [CAN MOCK] Bind failed! Port 8080 may be in use.\r\n");
        exit(EXIT_FAILURE);
    }

    // 3. Lắng nghe (待機します - たいきします)
    if (listen(server_fd, 3) < 0) {
        UART0_SendString("🔥 [CAN MOCK] Listen failed!\r\n");
        exit(EXIT_FAILURE);
    }

    UART0_SendString("🌐 [CAN MOCK] Server Listening on PORT 8080...\r\n");
    UART0_SendString("⏳ [CAN MOCK] Waiting for Python Client to Connect...\r\n");

    // 4. Chấp nhận kết nối (接続を受け入れます - せつぞくをうけいれます)
    // LƯU Ý: Hàm này sẽ BLOCK (chặn) toàn bộ hệ thống lại cho đến khi Python kết nối!
    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_socket < 0) {
        UART0_SendString("🔥 [CAN MOCK] Accept failed!\r\n");
        exit(EXIT_FAILURE);
    }

    // KỸ THUẬT LÕI: Chuyển socket sang chế độ Non-blocking (Không chặn)
    // Để Dispatcher có thể chạy với tốc độ ánh sáng mà không bị kẹt khi chờ dữ liệu
    int flags = fcntl(client_socket, F_GETFL, 0);
    fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

    UART0_SendString("✅ [CAN MOCK] Python Client Connected! TCP/IP Socket Ready.\r\n");
}


// ============================================================================
// NHẬN DỮ LIỆU (Đọc từ Mạng TCP/IP thay vì Bàn phím)
// ============================================================================
// Sửa int thành uint8_t
uint8_t CanIf_Receive(CanMessage_t *msg) {
    if (client_socket < 0) return 0;

    static char line_buffer[256];
    static int buf_idx = 0;
    char c;

    // Đọc từng byte một từ Mạng (受信します - じゅしんします)
    while (recv(client_socket, &c, 1, MSG_DONTWAIT) > 0) {
        if (c == '\n') {
            line_buffer[buf_idx] = '\0';
            buf_idx = 0; // Reset bộ đệm cho dòng sau

            // Bóc tách dữ liệu y hệt cũ
            unsigned int id;
            unsigned char dlc, data0;
            if (sscanf(line_buffer, "CAN %x %hhu %hhu", &id, &dlc, &data0) == 3) {
                msg->id = id;
                msg->dlc = dlc;
                msg->data[0] = data0;
                return 1; // Có dữ liệu chuẩn
            } else {
                // Tiêm lỗi: Ném ID rác để hệ thống báo DTC
                msg->id = 0x999; 
                msg->dlc = 0;
                return 1; 
            }
        } else if (c != '\r' && buf_idx < 255) {
            line_buffer[buf_idx++] = c;
        }
    }
    return 0; // Đường truyền đang trống
}

// ============================================================================
// GỬI PHẢN HỒI (Vẫn in ra Màn hình để Python thu thập Log)
// ============================================================================
// Sửa void thành uint8_t, và thêm từ khóa const
uint8_t CanIf_Transmit(const CanMessage_t *msg) {
    char buffer[100];
    sprintf(buffer, "[CAN MOCK] TX -> ID: 0x%X, DLC: %d, Data[0]: 0x%X\r\n", msg->id, msg->dlc, msg->data[0]);
    UART0_SendString(buffer); // Truyền thông (通信します - つうしんします)
    return 1; // Báo gửi thành công
}