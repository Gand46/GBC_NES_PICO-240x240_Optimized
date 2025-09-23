#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void DoomInputInit(void);
bool DoomInputConsumeExitRequest(void);
bool DoomInputConsumeMenuRequest(void);

#ifdef __cplusplus
}
#endif

