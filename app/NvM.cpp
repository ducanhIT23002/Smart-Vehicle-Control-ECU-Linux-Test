#include "NvM.hpp"
#include "uart.h"
#include <stdio.h>
#include <string.h>

NvM myNvM; 

NvM::NvM() {
    file_path = "eeprom_mock.dat"; // Tên file lưu trữ
    memset(&ram_mirror, 0, sizeof(ram_mirror)); // Xóa sạch RAM lúc mới cấp điện
}

void NvM::Init() {
    // Thư viện <stdio.h>: Mở file ở chế độ Read Binary (rb)
    FILE* file = fopen(file_path, "rb"); 
    
    if (file != NULL) {
        // Hàm fread: Kéo 1 cục data có kích thước NvM_DataBlock_t từ 'file' nhét vào biến '&ram_mirror'
        fread(&ram_mirror, sizeof(NvM_DataBlock_t), 1, file);
        fclose(file); // Đọc xong phải đóng lại để giải phóng file
        UART0_SendString("💾 [NvM] Restore data from EEPROM success!\r\n");
    } else {
        // Nếu file == NULL (Xe mới xuất xưởng, chưa có EEPROM)
        UART0_SendString("💾 [NvM] No EEPROM found. Create new one!\r\n");
        WriteAll(); // Gọi hàm tạo file mới với data RAM đang bằng 0
    }
}

void NvM::WriteAll() {
    // Mở file chế độ Write Binary (wb) - Tự động tạo file nếu chưa có, hoặc ghi đè (上書きします - うわがきします) nếu đã có
    FILE* file = fopen(file_path, "wb"); 
    if (file != NULL) {
        // Hàm fwrite: Lấy data từ '&ram_mirror', độ lớn NvM_DataBlock_t, xả xuống 'file'
        fwrite(&ram_mirror, sizeof(NvM_DataBlock_t), 1, file);
        fclose(file);
        UART0_SendString("💾 [NvM] Data saved to EEPROM!\r\n");
    }
}

void NvM::Inc_QueueFullError() {
    ram_mirror.dtc_queue_full_count++; // Tăng biến trên RAM
    WriteAll(); // Ép xả xuống EEPROM ngay lập tức (Sync)
}

uint16_t NvM::Get_QueueFullError() {
    return ram_mirror.dtc_queue_full_count; // Chỉ cần đọc từ RAM ra cho nhanh
}

void NvM::Clear_QueueFullError() {
    ram_mirror.dtc_queue_full_count = 0; // 1. Xóa sạch data trên RAM (Khởi tạo lại - 初期化します - しょきかします)
    WriteAll();                          // 2. Ép xả (Sync) cái sự "sạch sẽ" này đè xuống file EEPROM
}