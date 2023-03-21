#pragma once

#include "leia/common/api.h"

#include <stdint.h>

BEGIN_CAPI_DECL

enum // leia_log_level
{
    kLeiaLogLevelDefault = 0,
    kLeiaLogLevelTrace,
    kLeiaLogLevelDebug,
    kLeiaLogLevelInfo,
    kLeiaLogLevelWarn,
    kLeiaLogLevelError,
    kLeiaLogLevelCritical,
    kLeiaLogLevelOff,
    kNumLeiaLogLevels
};

typedef int32_t leia_log_level;

LEIA_COMMON_API
const char* leia_log_level_to_string(leia_log_level);

END_CAPI_DECL
