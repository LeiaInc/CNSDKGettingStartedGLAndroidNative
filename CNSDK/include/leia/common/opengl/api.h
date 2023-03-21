#pragma once

#include "leia/common/defines.h"

#ifdef LEIA_COMMON_OPENGL_EXPORTS
# define LEIA_COMMON_OPENGL_API LEIA_EXPORT
#else
# ifdef LEIA_COMMON_OPENGL_STATIC
#  define LEIA_COMMON_OPENGL_API
# else
#  define LEIA_COMMON_OPENGL_API LEIA_IMPORT
# endif
#endif
