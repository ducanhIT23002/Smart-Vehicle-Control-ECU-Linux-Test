import subprocess
import threading
import sys

# ============================================================================
# HÀM 1: LUỒNG CHUYÊN ĐỌC LOG (Chạy ngầm)
# ============================================================================
def read_logs(process):
    # Lặp vòng (ループします - るーぷします)
    for line in process.stdout:
        clean_line = line.strip()
        
        # Xóa dòng hiện tại và in log mới (tránh đè chữ lên chỗ nhập lệnh)
        sys.stdout.write('\r' + ' ' * 50 + '\r') 

        if "DOOR" in clean_line:
            print(f"🚪 {clean_line}")
        elif "WIPER" in clean_line:
            print(f"🌧️ {clean_line}")
        elif "LIGHT" in clean_line:
            print(f"💡 {clean_line}")
        elif "Dispatcher" in clean_line:
            print(f"📡 {clean_line}")
        else:
            print(f"   {clean_line}")
        
        # In lại con trỏ nhập lệnh
        sys.stdout.write(">> Nhập lệnh CAN: ")
        sys.stdout.flush()

# ============================================================================
# HÀM 2: LUỒNG CHÍNH (Giao diện nhập liệu)
# ============================================================================
def start_dashboard():
    print("=========================================")
    print("🚗 BCM VIRTUAL DASHBOARD V2.0")
    print("   [HDSD] Gõ lệnh theo format: CAN <ID> <DLC> <DATA>")
    print("   [VD] Mở cửa: CAN 100 1 1")
    print("=========================================\n")

    # Mở ống dẫn cả 2 chiều: stdin (Gửi lệnh) và stdout (Nhận log)
    process = subprocess.Popen(
        ['../bcm_sil_app'], 
        stdin=subprocess.PIPE,  
        stdout=subprocess.PIPE, 
        text=True               
    )

    # Đa luồng (マルチスレッド - まるちすれっど)
    # 例文: マルチスレッドでログを読(よ)み込(こ)みます。
    # (Đọc log bằng đa luồng.)
    # Khởi tạo một luồng phụ tách biệt để chạy hàm read_logs ở trên
    reader_thread = threading.Thread(target=read_logs, args=(process,), daemon=True)
    reader_thread.start()

    # Luồng chính đứng đây chờ ông gõ phím
    try:
        while True:
            # Chờ người dùng nhập (入力 - にゅうりょく)
            cmd = input(">> Nhập lệnh CAN: ")
            
            if cmd.lower() in ['exit', 'quit']:
                break
                
            # Bơm lệnh vào ống dẫn cho C đọc
            process.stdin.write(cmd + "\n")
            process.stdin.flush()

    except KeyboardInterrupt:
        pass
    finally:
        print("\n🛑 SHUTTING DOWN DASHBOARD...")
        process.terminate()

if __name__ == "__main__":
    start_dashboard()