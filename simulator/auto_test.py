import subprocess
import time
import os
import fcntl

def run_simulation():
    print("🚀 [Python] Bắt đầu tự động hóa Test BCM...")
    
    # Khởi chạy (起動します - きどうします)
    # 例文: Cのプログラムを起動(きどう)します。
    # (Khởi chạy chương trình C.)
    # Subprocess.Popen sẽ tạo một tiến trình con chạy file thực thi bcm_sil_app độc lập với Python.
    process = subprocess.Popen(
        ['../bcm_sil_app'], 
        stdin=subprocess.PIPE, 
        stdout=subprocess.PIPE, 
        text=True
    )

    # Lấy ra (取得します - しゅとくします)
    # 例文: ファイルディスクリプタを取得(しゅとく)します。
    # (Lấy ra file descriptor - định danh quản lý tệp.)
    fd = process.stdout.fileno()
    fl = fcntl.fcntl(fd, fcntl.F_GETFL)
    
    # Thiết lập (設定します - せっていします)
    # 例文: パイプを非同期(ひどうき)に設定(せってい)します。
    # (Thiết lập đường ống thành không chặn/non-blocking.)
    fcntl.fcntl(fd, fcntl.F_SETFL, fl | os.O_NONBLOCK)

    time.sleep(0.5) 
    print("✅ [Python] Đã bật hệ thống C. Các luồng đang chạy...")

    # Ghi / Nhập liệu (書き込みます - かきこみます)
    # 例文: データを標準入力(ひょうじゅんにゅうりょく)に書(か)き込(こ)みます。
    # (Ghi dữ liệu vào đầu vào tiêu chuẩn stdin.)
    print("\n⚡ [Python] TIÊM LỆNH 1: Mở Cửa (CAN ID: 0x100, Data: 1)...")
    process.stdin.write("CAN 100 1 1\n") 
    process.stdin.flush()
    time.sleep(0.2)

    print("\n⚡ [Python] TIÊM LỆNH 2: Bấm nút Gạt Mưa (CAN ID: 0x200, Data: 3)...")
    process.stdin.write("CAN 200 1 3\n") 
    process.stdin.flush()
    time.sleep(0.2)

    print("\n⚡ [Python] TIÊM LỆNH 3: Bật Đèn (CAN ID: 0x300, Data: 4)...")
    process.stdin.write("CAN 300 1 4\n") 
    process.stdin.flush()
    time.sleep(0.5) 
    
    print("\n👀 [Python] Đọc toàn bộ phản hồi từ các Task C...")
    
    while True:
        try:
            # Đọc (読み込みます - よみこみます)
            # 例文: 結果(けっか)を読(よ)み込(こ)みます。
            # (Đọc kết quả trả về.)
            line = process.stdout.readline()
            if not line: break
            print(f"  [C LOG] {line.strip()}")
        except TypeError:
            break
        except Exception:
            break

    # Chấm dứt / Tắt (終了します - しゅうりょうします)
    # 例文: プロセスを終了(しゅうりょう)します。
    # (Chấm dứt tiến trình.)
    process.terminate()

if __name__ == "__main__":
    run_simulation()
# import subprocess
# import time
# import os
# import fcntl

# def run_simulation():
#     print("🚀 [Python] Bắt đầu tự động hóa (自動化) Test BCM...")
    
#     process = subprocess.Popen(
#         ['../bcm_sil_app'], 
#         stdin=subprocess.PIPE, 
#         stdout=subprocess.PIPE, 
#         text=True
#     )

#     # Thủ thuật giúp Python không bị treo khi đọc log (Set Non-blocking)
#     fd = process.stdout.fileno()
#     fl = fcntl.fcntl(fd, fcntl.F_GETFL)
#     fcntl.fcntl(fd, fcntl.F_SETFL, fl | os.O_NONBLOCK)

#     time.sleep(0.5) # Đợi hệ thống C boot xong
    
#     print("✅ [Python] Đã bật hệ thống C. Các luồng đang chạy...")
#     print("-" * 50)

#     # --- ĐOẠN TIÊM LỆNH THỜI GIAN THỰC ---
#     # Mã Data 1: SYS_EVT_DOOR_OPENED
#     print("\n⚡ [Python] TIÊM LỆNH 1: Mở Cửa (CAN ID: 0x100, Data: 1)...")
#     process.stdin.write("CAN 100 1 1\n") 
#     process.stdin.flush()
#     time.sleep(0.2)

#     # Mã Data 3: SYS_EVT_WIPER_BTN_PRESSED
#     print("\n⚡ [Python] TIÊM LỆNH 2: Bấm nút Gạt Mưa (CAN ID: 0x200, Data: 3)...")
#     process.stdin.write("CAN 200 1 3\n") 
#     process.stdin.flush()
#     time.sleep(0.2)

#     # Mã Data 4: SYS_EVT_DIMMER_BTN_PRESSED
#     print("\n⚡ [Python] TIÊM LỆNH 3: Bật Đèn (CAN ID: 0x300, Data: 4)...")
#     process.stdin.write("CAN 300 1 4\n") 
#     process.stdin.flush()
#     time.sleep(0.5) # Đợi C xử lý nốt
    
#     print("\n👀 [Python] Đọc toàn bộ phản hồi từ các Task C...")
#     # Quét sạch ống Output, có bao nhiêu chữ in ra bấy nhiêu, không bị treo nữa
#     while True:
#         try:
#             line = process.stdout.readline()
#             if not line: break
#             print(f"  [C LOG] {line.strip()}")
#         except TypeError:
#             # Hết log để đọc
#             break
#         except Exception:
#             break

#     print("-" * 50)
#     print("🛑 [Python] Hoàn thành bài test tự động. Tắt hệ thống.")
#     process.terminate()

# if __name__ == "__main__":
#     run_simulation()