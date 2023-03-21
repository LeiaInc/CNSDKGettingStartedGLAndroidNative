#pragma once

#include "leia/common/api.h"

#include <string>
#include <memory>

namespace leia {

#if defined(LEIA_OS_ANDROID)
using NativePath = std::string;
#elif defined(LEIA_OS_WINDOWS)
using NativePath = std::wstring;
#endif

class Platform;

class AssetManager {
public:
    LEIA_COMMON_API
    static std::unique_ptr<AssetManager> Create(Platform*);
    LEIA_COMMON_API
    virtual ~AssetManager();

    struct ResolveContext {
#if defined(LEIA_OS_WINDOWS)
        /// If specified, used as a base directory for relative assetPaths.
        NativePath baseDir;
        /// Allows automatic base dir evaluation based on an address from a module (.dll, .exe).
        /// See windowsPlatform's GetLibraryPath(void*).
        void* moduleAddress = nullptr;
#endif
    };

    // TODO: add function that returns read buffer (std::fstream::rdbuf) for efficient .tga parsing

    struct Buffer {
        size_t size;
        std::unique_ptr<char[]> data;
    };
    virtual Buffer ReadData(std::string const& assetPath, ResolveContext* resolveContext = nullptr) = 0;

    virtual std::string ReadString(std::string const& assetPath, ResolveContext* resolveContext = nullptr) = 0;

    virtual NativePath UnpackDir(std::string const& dirName, ResolveContext* resolveContext = nullptr) = 0;

protected:
    AssetManager();
};

#if defined(LEIA_OS_ANDROID)
NativePath const& GetCacheDir(AssetManager*);
#endif

} // namespace leia
