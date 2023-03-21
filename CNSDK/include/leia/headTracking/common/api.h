#pragma once

#include "leia/common/defines.h"

#ifdef LHT_COMMON_EXPORTS
# define LHT_COMMON_API LEIA_EXPORT
#else
# ifdef LHT_COMMON_STATIC
#  define LHT_COMMON_API
# else
#  define LHT_COMMON_API LEIA_IMPORT
# endif
#endif
