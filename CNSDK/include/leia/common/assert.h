#pragma once

#include "leia/common/api.h"

#if defined(LEIA_COMPILER_MSVC)
# define LNK_PRETTY_FUNCTION __FUNCSIG__
#elif defined(LEIA_COMPILER_CLANG)
# define LNK_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
# error "Unknown compiler"
#endif

#ifndef LNK_ENABLE_ASSERT
# define LNK_ASSERT(e) ((void)0)
#else
# define LNK_ASSERT(e) ((e) ? (void)0 : leiaAssert(__FILE__, __LINE__, LNK_PRETTY_FUNCTION, #e))
#endif

[[noreturn]]
LEIA_COMMON_API
void leiaAssert(const char* file, int line, const char* function, const char* msg);
