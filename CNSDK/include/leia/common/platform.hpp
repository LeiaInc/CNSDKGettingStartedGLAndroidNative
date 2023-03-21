#pragma once

#include "leia/common/api.h"
#include "leia/common/log.h"
#include "leia/common/jniTypes.h"

#include <memory>

namespace leia {

struct PlatformInitArgs {
    /// Android activity (android.app.Activity) is required to bind to head tracking service.
    /// Unity and Unreal applications can leave this field set to nullptr.
    jobject androidActivity = nullptr;

    /// Android context (android.content.Context) is required to correctly set up Blink resources embedded into .apk.
    /// Unity and Unreal applications can leave this field set to nullptr.
    jobject androidContext = nullptr;

    /// Java VM handle. Should be specified in the purely native android applications when JNI_OnLoad is not called.
    JavaVM* javaVM = nullptr;

    /// Use any kLeiaLogLevel except the default one to override the logging level.
    leia_log_level logLevel = kLeiaLogLevelDefault;
};

class Platform {
public:
    LEIA_COMMON_API
    virtual ~Platform();

    LEIA_COMMON_API
    static std::unique_ptr<Platform> Create(PlatformInitArgs const& initArgs);

    // Returns time in nanoseconds with the highest possible precision of the system steady clock.
    // Android: android.os.SystemClock.elapsedRealtimeNanos.
    // Windows: std::chrono::system_clock.
    virtual uint64_t GetSystemTimeNs() const = 0;

    LEIA_COMMON_API
    static void OnLibraryLoad();
    LEIA_COMMON_API
    static void OnLibraryUnload();

private:
    static void InitLogging();
    static void DeinitLogging();
};

} // namespace leia
