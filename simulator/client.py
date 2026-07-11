import socket
import sys
import time

HOST = '127.0.0.1' 
PORT = 8080

def start_client():
    print("======================================================")
    print("🌐 BCM TESTER TOOL V5.0 (REQUIREMENT-DRIVEN EDITION)")
    print(f"   Đang tìm kiếm ECU tại {HOST}:{PORT}...")
    print("======================================================\n")

    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((HOST, PORT))
        print("✅ ĐÃ KẾT NỐI THÀNH CÔNG VỚI BCM SERVER!\n")
    except ConnectionRefusedError:
        print("🔥 [LỖI] Không thể kết nối! Hãy chắc chắn BCM C++ đang chạy.")
        sys.exit(1)

    print("--- CÁC LỆNH KIỂM THỬ NHANH (REQUIREMENTS) ---")
    print("  FR1 : Test FR-001 (Chạy tốc độ 25km/h -> Khóa cửa)")
    print("  FR1_STOP : Test FR-001 (Dừng xe 0km/h -> Mở cửa)")
    print("  FR2 : Test FR-002 (Giả lập va chạm khẩn cấp)")
    print("  FR3 : Test FR-003 (Bật gạt mưa liên tục 5s -> Bật đèn)")
    print("  FR4 : Test FR-004 (Đóng cửa -> Chờ 5s tắt đèn trần)")
    print("  SPAM 50 : Tấn công DoS / UDS READ : Đọc lỗi UDS")
    print("----------------------------------------------")

    try:
        while True:
            cmd = input("\n>> Nhập lệnh (hoặc mã CAN): ")
            cmd_upper = cmd.upper()
            
            if cmd_upper in ['EXIT', 'QUIT']:
                break
                
            # ================= CÁC BÀI TEST NGHIỆP VỤ =================
            elif cmd_upper == 'FR1':
                print("🚙 [FR-001] Đang đạp ga lên 25km/h...")
                client_socket.sendall(b"CAN 0C0 1 25\n")
            elif cmd_upper == 'FR1_STOP':
                print("🛑 [FR-001] Đang phanh xe dừng hẳn (0km/h)...")
                client_socket.sendall(b"CAN 0C0 1 0\n")
            elif cmd_upper == 'FR2':
                print("💥 [FR-002] Gửi tín hiệu Va chạm (Crash)...")
                client_socket.sendall(b"CAN 100 1 255\n") # Bắn vào Task Cửa
            elif cmd_upper == 'FR3':
                print("🌧️ [FR-003] Bật gạt mưa. Hãy chờ 5s để ECU xử lý...")
                client_socket.sendall(b"CAN 200 1 3\n")
            elif cmd_upper == 'FR4':
                print("🚪 [FR-004] Đóng cửa vật lý. Hãy đếm lùi 5s xem đèn tắt không...")
                client_socket.sendall(b"CAN 100 1 2\n")
            
            # ================= CÁC BÀI TEST BẢO MẬT/CHẨN ĐOÁN =================
            elif cmd_upper.startswith('SPAM'):
                count = int(cmd.split()[1])
                print(f"⚠️ ĐANG BƠM {count} LỆNH QUA MẠNG LAN...")
                for _ in range(count):
                    client_socket.sendall(b"CAN 100 1 1\n")
            elif cmd_upper == 'UDS READ':
                print("🛠️ [UDS] Yêu cầu đọc lỗi (Read DTC 0x19)...")
                client_socket.sendall(b"CAN 7E0 8 25\n")
            elif cmd_upper == 'UDS CLEAR':
                print("🧹 [UDS] Yêu cầu xóa lỗi (Clear DTC 0x14)...")
                client_socket.sendall(b"CAN 7E0 8 20\n")
            else:
                client_socket.sendall((cmd + "\n").encode('utf-8'))

    except KeyboardInterrupt:
        pass
    finally:
        print("\n🛑 NGẮT KẾT NỐI...")
        client_socket.close()

if __name__ == "__main__":
    start_client()