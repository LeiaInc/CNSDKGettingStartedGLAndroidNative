#pragma once

#include "leia/common/defines.h"

#ifdef LEIADEVICE_EXPORTS
# define LEIADEVICE_API LEIA_EXPORT
#else
# ifdef LEIADEVICE_STATIC
#  define LEIADEVICE_API
# else
#  define LEIADEVICE_API LEIA_IMPORT
# endif
#endif
