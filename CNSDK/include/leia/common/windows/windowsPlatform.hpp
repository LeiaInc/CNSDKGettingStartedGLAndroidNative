#pragma once

#include "leia/common/platform.hpp"
#include "leia/common/windows/windowsFilesystem.hpp"

namespace leia {

class WindowsPlatform : public Platform {
public:
    WindowsPlatform(PlatformInitArgs const&);
    ~WindowsPlatform() override = default;

    uint64_t GetSystemTimeNs() const override;

    static void OnLibraryLoad();
    static void OnLibraryUnload();

    static void InitLogging();
};

LEIA_COMMON_API
fs::path GetDefaultLogsDirectory();
LEIA_COMMON_API
fs::path GetExecutablePath();
// Gets a path to a module that contains the \p libraryAddress.
// See GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS for more info.
LEIA_COMMON_API
fs::path GetLibraryPath(void* libraryAddress);
// Gets a path to this module.
LEIA_COMMON_API
fs::path GetLibraryPath();
LEIA_COMMON_API
fs::path GetAppDataPath();
LEIA_COMMON_API
std::string GetLastErrorAsString();

} // namespace leia
