#pragma once

#include "leia/common/defines.h"

#include <stdint.h>

#if defined(LEIA_OS_ANDROID)
#include <jni.h>
#else
class _jobject {};
using jobject = _jobject*;
struct _JavaVM;
using JavaVM = _JavaVM;
struct _JNIEnv;
using JNIEnv = _JNIEnv;
using jint = int32_t;
#endif // LEIA_OS_ANDROID
