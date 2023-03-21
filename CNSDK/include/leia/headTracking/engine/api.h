#pragma once

#include "leia/headTracking/common/api.h"

#ifdef LHT_ENGINE_EXPORTS
# define LHT_ENGINE_API LEIA_EXPORT
#else
# ifdef LHT_ENGINE_STATIC
#  define LHT_ENGINE_API
# else
#  define LHT_ENGINE_API LEIA_IMPORT
# endif
#endif

#include "leia/headTracking/engine/config.h"
