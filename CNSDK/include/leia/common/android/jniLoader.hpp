#pragma once

#include "leia/common/jniTypes.h"

namespace leia {
namespace jni {

void InitNativeApp(JavaVM* vm);
void DeinitNativeApp();

jint OnLoad(JavaVM* vm);
void OnUnload(JavaVM* vm);

bool IsInitialized();
JavaVM* GetJavaVM();

} // namespace jni
} // namespace leia
