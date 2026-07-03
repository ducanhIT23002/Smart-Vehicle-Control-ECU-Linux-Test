import socket
import sys
import time

# Địa chỉ IP của Máy chủ (Localhost) và Cổng kết nối
HOST = '127.0.0.1' 
PORT = 8080

def start_client():
    print("======================================================")
    print("🌐 BCM TESTER TOOL V4.0 (TCP/IP CONNECTED ECU)")
    print(f"   Đang tìm kiếm ECU tại {HOST}:{PORT}...")
    print("======================================================\n")

    try:
        # 1. Tạo Socket mạng
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # 2. Yêu cầu kết nối (接続します - せつぞくします)
        client_socket.connect((HOST, PORT))
        print("✅ ĐÃ KẾT NỐI THÀNH CÔNG VỚI BCM SERVER!\n")
    except ConnectionRefusedError:
        print("🔥 [LỖI] Không thể kết nối! Hãy chắc chắn BCM C++ đang chạy ở một Terminal khác.")
        sys.exit(1)

    try:
        while True:
            cmd = input(">> Lệnh CAN (CAN/SPAM/NOISE/EXIT): ")
            cmd_upper = cmd.upper()
            
            if cmd_upper in ['EXIT', 'QUIT']:
                break
                
            elif cmd_upper.startswith('SPAM'):
                try:
                    count = int(cmd.split()[1])
                    print(f"⚠️ ĐANG BƠM {count} LỆNH QUA MẠNG LAN...")
                    # Gửi dữ liệu (送信します - そうしんします)
                    for _ in range(count):
                        # Mạng TCP yêu cầu phải chuyển chuỗi (String) thành Byte (b"...")
                        client_socket.sendall(b"CAN 100 1 1\n")
                        client_socket.sendall(b"CAN 200 1 3\n")
                    print("✅ ĐÃ GỬI XONG QUA MẠNG!\n")
                except:
                    print("❌ Cú pháp sai. VD: SPAM 100")
                    
            elif cmd_upper == 'NOISE':
                print("⚠️ ĐANG TIÊM DỮ LIỆU RÁC (NOISE) QUA MẠNG...")
                client_socket.sendall(b"CAN 999 9 99\n")
                client_socket.sendall(b"GARBAGE DATA NETWORK ERROR\n")
            # ==========================================================
            # THÊM MỚI: MÁY QUÉT LỖI UDS
            # ==========================================================
            elif cmd_upper == 'UDS READ':
                print("🛠️ [UDS] Đang yêu cầu BCM trích xuất lịch sử lỗi (Read DTC 0x19)...")
                # Gửi CAN ID 0x7E0. Byte đầu tiên là 25 (Chính là 0x19 trong hệ Thập phân)
                client_socket.sendall(b"CAN 7E0 8 25\n")
            
            # ==========================================================
            # THÊM MỚI: LỆNH XÓA LỖI (CLEAR DTC 0x14)
            # ==========================================================
            elif cmd_upper == 'UDS CLEAR':
                print("🧹 [UDS] Đang yêu cầu BCM xóa sạch lịch sử lỗi (Clear DTC 0x14)...")
                # Gửi CAN ID 0x7E0. Số 20 (Thập phân) chính là 0x14 trong hệ Hex.
                client_socket.sendall(b"CAN 7E0 8 20\n")
            
            elif cmd_upper == 'CRASH LIGHT':
                print("💀 [SAFETY TEST] Đang bắn mã độc làm treo Task Đèn Pha...")
                # Điền số thập phân của SYS_EVT_CRASH vào chỗ [SỐ_CỦA_ÔNG]
                client_socket.sendall(b"CAN 300 1 255\n")

            else:
                # Gửi lệnh bình thường đã được mã hóa thành Byte
                client_socket.sendall((cmd + "\n").encode('utf-8'))

    except KeyboardInterrupt:
        pass
    finally:
        print("\n🛑 NGẮT KẾT NỐI (切断します - せつだんします)...")
        client_socket.close()

if __name__ == "__main__":
    start_client()