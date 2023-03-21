#pragma once

#include "leia/common/defines.h"

#ifdef LEIASDK_EXPORTS
# define LEIASDK_API LEIA_EXPORT
# define LEIASDK_CLASS_API LEIA_CLASS_EXPORT
#else
# ifdef LEIASDK_STATIC
#  define LEIASDK_API
#  define LEIASDK_CLASS_API
# else
#  define LEIASDK_API LEIA_IMPORT
#  define LEIASDK_CLASS_API LEIA_CLASS_IMPORT
# endif
#endif
