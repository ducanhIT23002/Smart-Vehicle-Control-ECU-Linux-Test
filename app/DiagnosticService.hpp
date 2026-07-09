#ifndef DIAGNOSTIC_SERVICE_HPP
#define DIAGNOSTIC_SERVICE_HPP

#include "can_if.h"

class DiagnosticService {
public:
    DiagnosticService();
    // Hàm nhận vào gói tin CAN và xử lý nếu nó là lệnh UDS
    void ProcessRequest(const CanMessage_t* rx_msg); 
};

#endif // DIAGNOSTIC_SERVICE_HPP