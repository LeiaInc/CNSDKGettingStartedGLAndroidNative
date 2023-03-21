#pragma once

#include "leia/common/defines.h"

#if defined(LEIA_OS_WINDOWS)
#include "leia/common/windows/windowsFilesystem.hpp"
#elif defined(LEIA_OS_ANDROID)
#include "leia/common/android/androidFilesystem.hpp"
#endif
