#pragma once

#include "leia/headTracking/common/api.h"

#if defined(LHT_SERVICE_STATIC)
# define LHT_SERVICE_API
# define LHT_SERVICE_API_CLASS
#else
# ifdef LHT_SERVICE_EXPORTS
#   define LHT_SERVICE_API LEIA_EXPORT
#  else
#   define LHT_SERVICE_API LEIA_IMPORT
# endif

# ifdef LHT_COMPILER_MSVC
#  define LHT_SERVICE_API_CLASS
# else
#  define LHT_SERVICE_API_CLASS LHT_SERVICE_API
# endif
#endif

#include "leia/headTracking/service/config.h"
