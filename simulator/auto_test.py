import subprocess
import socket
import time

APP_PATH = '../bcm_sil_app'
HOST = '127.0.0.1'
PORT = 8080
REPORT_FILE = 'Test_Report.txt'

def run_auto_test():
    print("🚀 [AUTO-TEST] Khởi động C++ BCM ngầm bên dưới HĐH...")
    process = subprocess.Popen([APP_PATH], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    time.sleep(2)

    print("🌐 [AUTO-TEST] Đang kết nối TCP/IP tới ECU...")
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((HOST, PORT))
    except Exception as e:
        print(f"❌ FATAL ERROR: {e}")
        process.terminate()
        return

    # ---------------------------------------------------------
    print("\n▶️ [TC-01] Kiểm thử FR-001: Auto Door Lock")
    client.sendall(b"CAN 0C0 1 25\n") # Tốc độ 25km/h
    time.sleep(1)
    client.sendall(b"CAN 0C0 1 0\n")  # Tốc độ 0km/h
    time.sleep(1)

    # ---------------------------------------------------------
    print("▶️ [TC-02] Kiểm thử FR-003 & FR-004: Delay Timer 5s")
    client.sendall(b"CAN 100 1 1\n") # Mở cửa
    time.sleep(0.5)
    client.sendall(b"CAN 100 1 2\n") # Đóng cửa (Kích hoạt FR-004 Fade-out)
    time.sleep(0.5)
    client.sendall(b"CAN 200 1 3\n") # Bật gạt mưa (Kích hoạt FR-003 Light Sync)
    
    print("   ⏳ Đang chờ 6 giây để ECU xử lý State Machine (Fade-out & Rain Sync)...")
    time.sleep(12) 

    # ---------------------------------------------------------
    print("▶️ [TC-03] Kiểm thử FR-002: Crash Override")
    client.sendall(b"CAN 100 1 255\n") # Gửi Crash Event
    time.sleep(1)

    # ---------------------------------------------------------
    print("▶️ [TC-04] Kiểm thử Bảo mật: Bơm SPAM tấn công DoS")
    for _ in range(50):
        client.sendall(b"CAN 100 1 1\n")
    
    print("⏳ Đợi xử lý dữ liệu cuối cùng...")
    time.sleep(2)
    client.close()

    try:
        process.wait(timeout=15)
    except subprocess.TimeoutExpired:
        process.terminate()

    stdout, _ = process.communicate()

    print("\n📊 [AUTO-TEST] Đang chấm điểm Requirements...")
    # Chấm điểm dựa trên log in ra từ C++ (Phải chính xác tuyệt đối từng chữ)
    tc1_pass = "AUTO-LOCKING" in stdout and "AUTO-UNLOCKING" in stdout
    tc2_pass = "Faded Out Completely" in stdout and "AUTO HEADLIGHT ON" in stdout
    tc3_pass = "CRASH RECEIVED" in stdout or "Auto-unlocking all doors" in stdout
    tc4_pass = "Rate limit exceeded" in stdout

    with open(REPORT_FILE, 'w', encoding='utf-8') as f:
        f.write("==================================================\n")
        f.write("    BÁO CÁO NGHIỆM THU PRODUCT REQUIREMENTS       \n")
        f.write("==================================================\n\n")
        f.write(f"[FR-001] Khóa/Mở tự động theo tốc độ: {'✅ PASS' if tc1_pass else '❌ FAIL'}\n")
        f.write(f"[FR-003 & 004] Timer 5s (Đèn & Gạt mưa):  {'✅ PASS' if tc2_pass else '❌ FAIL'}\n")
        f.write(f"[FR-002] An toàn va chạm (Crash Override):{'✅ PASS' if tc3_pass else '❌ FAIL'}\n")
        f.write(f"[SEC-01] Phòng chống Hacker DoS:          {'✅ PASS' if tc4_pass else '❌ FAIL'}\n\n")
        f.write("==================================================\n")
        f.write("--- DỮ LIỆU LOG THÔ TỪ HỆ THỐNG C++ ---\n")
        f.write("==================================================\n")
        f.write(stdout)

    print(f"✅ Hoàn tất! Đã xuất báo cáo tại: {REPORT_FILE}")

if __name__ == '__main__':
    run_auto_test()