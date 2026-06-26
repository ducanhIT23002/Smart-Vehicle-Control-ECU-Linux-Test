#include "can_if.h"
#include <stdio.h>
#include <string.h>

// ============================================================================
// HÀM 1: KHỞI TẠO GIAO DIỆN CAN
// ============================================================================
void CanIf_Init(void) {
    // Khởi tạo (初期化します - しょきかします)
    // 例文: CANインターフェースを初期化(しょきか)します。 
    // (Khởi tạo giao diện CAN.)
    printf("[CAN MOCK] CAN Interface Initialized.\n");
}

// ============================================================================
// HÀM 2: TRUYỀN DỮ LIỆU TỪ HỆ THỐNG RA NGOÀI
// Logic: Thay vì truyền điện áp, ta dùng printf in ra Terminal để Python đọc.
// ============================================================================
uint8_t CanIf_Transmit(const CanMessage_t *msg) {
    // Truyền / Gửi (送信します - そうしんします)
    // 例文: 画面(がめん)にCANデータを送信(そうしん)します。 
    // (Truyền dữ liệu CAN ra màn hình.)
    printf("[CAN MOCK] TX -> ID: 0x%X, DLC: %d, Data[0]: 0x%X\n", msg->id, msg->dlc, msg->data[0]);
    
    // Trả về (返します - かえします)
    // 例文: 成功(せいこう)のステータスを返(かえ)します。 
    // (Trả về trạng thái thành công.)
    return 1; 
}

// ============================================================================
// HÀM 3: NHẬN DỮ LIỆU TỪ NGOÀI (PYTHON) VÀO HỆ THỐNG
// Logic: Hàm fgets() sẽ chặn (block) luồng cho đến khi Python đẩy lệnh vào.
// ============================================================================
uint8_t CanIf_Receive(CanMessage_t *msg) {
    char buffer[64];
    
    // Đọc (読み込みます - よみこみます)
    // 例文: パイプから文字列(もじれつ)を読(よ)み込(こ)みます。 
    // (Đọc chuỗi string từ đường ống pipe.)
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        
        uint32_t id = 0;
        int dlc = 0;
        int data0 = 0;
        
        // Phân tích (分析します - ぶんせきします)
        // 例文: 受信(じゅしん)した文字列(もじれつ)を分析(ぶんせき)します。 
        // (Phân tích chuỗi string đã nhận.)
        // Hàm sscanf bóc tách chuỗi định dạng "CAN %x %d %d" thành các số.
        if (sscanf(buffer, "CAN %x %d %d", &id, &dlc, &data0) == 3) {
            
            // Gán (代入します - だいにゅうします)
            // 例文: 構造体(こうぞうたい)に値(あたい)を代入(だいにゅう)します。 
            // (Gán giá trị vào struct.)
            msg->id = id;
            msg->dlc = dlc;
            msg->data[0] = (uint8_t)data0;
            
            // Nhận (受信します - じゅしんします)
            // 例文: Pythonからメッセージを受信(じゅしん)します。 
            // (Nhận bản tin từ Python.)
            printf("[CAN MOCK] RX -> ID: 0x%X, DLC: %d, Data: %d\n", msg->id, msg->dlc, msg->data[0]);
            
            return 1; // Báo hiệu đã có dữ liệu hợp lệ
        }
    }
    
    // Bỏ qua (無視します - むしします)
    // 例文: 無効(むこう)なデータを無視(むし)します。 
    // (Bỏ qua dữ liệu không hợp lệ.)
    return 0; 
}