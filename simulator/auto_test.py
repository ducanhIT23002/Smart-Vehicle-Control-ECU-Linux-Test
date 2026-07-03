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

    print("\n▶️ [SCENARIO 1] Chức năng cơ bản (Cửa & Gạt mưa)")
    client.sendall(b"CAN 100 1 1\n")
    time.sleep(0.5)
    client.sendall(b"CAN 200 1 3\n") # 3 là WIPER_BTN_PRESSED
    time.sleep(0.5)

    print("▶️ [SCENARIO 2] Cybersecurity - Bơm SPAM tấn công DoS")
    for _ in range(50):
        client.sendall(b"CAN 100 1 1\n")
    time.sleep(1)

    print("▶️ [SCENARIO 3] Functional Safety - Tiêm mã độc Deadlock")
    client.sendall(b"CAN 300 1 255\n") # 255 là SYS_EVT_CRASH

    print("⏳ Đợi Watchdog phát hiện Task chết và kích hoạt Fail-Safe (Tối đa 10s)...")
    client.close()

    try:
        process.wait(timeout=10)
    except subprocess.TimeoutExpired:
        print("⚠️ [WARNING] C++ không tự sập, ép buộc tiêu diệt (SIGTERM)...")
        process.terminate()

    stdout, _ = process.communicate()

    print("\n📊 [AUTO-TEST] Đang phân tích Log để chấm điểm...")
    tc1_pass = "DOOR OPEN -> LED ON" in stdout
    tc2_pass = "Mode INT" in stdout
    tc3_pass = "Rate limit exceeded" in stdout
    tc4_pass = "KÍCH HOẠT AN TOÀN! ÉP BẬT SÁNG ĐÈN PHA" in stdout

    with open(REPORT_FILE, 'w', encoding='utf-8') as f:
        f.write("==================================================\n")
        f.write("      BÁO CÁO NGHIỆM THU - SMART BCM SiL          \n")
        f.write("==================================================\n\n")
        f.write(f"[TC01] Đóng/Mở cửa:         {'✅ PASS' if tc1_pass else '❌ FAIL'}\n")
        f.write(f"[TC02] Điều khiển Gạt mưa:  {'✅ PASS' if tc2_pass else '❌ FAIL'}\n")
        f.write(f"[TC03] Cybersecurity (DoS): {'✅ PASS' if tc3_pass else '❌ FAIL'}\n")
        f.write(f"[TC04] Fail-Safe (Đèn pha): {'✅ PASS' if tc4_pass else '❌ FAIL'}\n\n")
        f.write("==================================================\n")
        f.write("--- DỮ LIỆU LOG THÔ TỪ HỆ THỐNG C++ ---\n")
        f.write("==================================================\n")
        f.write(stdout)

    print(f"✅ Hoàn tất! Đã xuất báo cáo tại: {REPORT_FILE}")

if __name__ == '__main__':
    run_auto_test()