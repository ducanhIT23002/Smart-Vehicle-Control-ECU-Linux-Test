#ifndef WDG_MANAGER_H
#define WDG_MANAGER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 定義します (ていぎします - định nghĩa) các mã định danh tác vụ cho Watchdog.
// 例文: ヘッダーファイルの中にタスクIDを定義します。(Tôi định nghĩa các ID của task vào trong file header.)
#define WDG_INPUT_TASK_ID  1
#define WDG_DOOR_TASK_ID   2
#define WDG_WIPER_TASK_ID  3
#define WDG_LIGHT_TASK_ID  4

void WdgM_Init(void);
void WdgM_CheckpointReached(uint32_t task_id);
void WdgM_Task(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* WDG_MANAGER_H */