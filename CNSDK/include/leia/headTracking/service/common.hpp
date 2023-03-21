#pragma once

#include "leia/headTracking/common/types.hpp"
#include "leia/common/log.h"

#include <string>

namespace leia {
namespace head {

enum ServiceSpecificError {
    kServiceSuccess,
    kInvalidFrame,
    kFailedToRegisterClient,
    kFailedToUnregisterClient,
    kInvalidClientId,
    kFailedToInitializeEngine,
    kFailedToStartTracking,
    kFailedToStopTracking,
    kInvalidValue,
    kInvalidProtocol,
    kNotSupported,
    kGenericError,
    kCameraPermissionDenied,
    kCommunicationBackendError,
};

struct ServiceError {
    ServiceSpecificError code;
    std::string message;
};

struct ServerInfo {
    std::string version;
    uint32_t supportedFaceDetectorBackends;
};

struct ClientServerInitArgs {
    leia_log_level logLevel = kLeiaLogLevelDefault;
    FaceDetectorConfig faceDetectorConfig = {kLeiaFaceDetectorBackendUnknown, kLeiaFaceDetectorInputTypeUnknown};
    bool startTracking = false;
    bool enableProfiling = false;

    head::SingleFaceConfiguration singleFaceConfig;
    int maxNumOfDetectedFaces = 0;
};

const char* ToStr(ServiceSpecificError error);

} // namespace head
} // namespace leia
