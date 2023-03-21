#pragma once

#include "leia/headTracking/service/api.h"
#include "leia/headTracking/service/common.hpp"
#include "leia/common/platform.hpp"

#include <functional>
#include <memory>

namespace leia {
namespace head {

class FrameAdapter;

namespace service {

struct ClientListener {
    virtual void OnConnect(ServerInfo const&) = 0;
    virtual void OnDisconnect() = 0;
    virtual void OnError(ServiceSpecificError error, std::string const& message) = 0;
    virtual void OnFaceDetectorConfigUpdate(FaceDetectorConfig) = 0;
};

struct ClientInitArgs {
    FrameAdapter* frameAdapter = nullptr;
    Platform* platform = nullptr;
    JNIEnv* jniEnv = nullptr;

    ClientListener* listener = nullptr;
    TrackingStateListener* trackingStateListener = nullptr;

    ClientServerInitArgs server;
};

class Client {
public:
    LHT_SERVICE_API
    Client(ClientInitArgs const&);
    LHT_SERVICE_API
    ~Client();

    LHT_SERVICE_API
    void StartTracking();
    LHT_SERVICE_API
    void StopTracking();

    LHT_SERVICE_API
    void SetFaceDetectorConfig(FaceDetectorConfig);
    LHT_SERVICE_API
    void SetServerLogLevel(leia_log_level);
    LHT_SERVICE_API
    void SetProfiling(bool enable);
    LHT_SERVICE_API
    void SetMaxNumOfDetectedFaces(int);
    LHT_SERVICE_API
    void SetSingleFaceConfig(SingleFaceConfiguration const&);

private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace service
} // namespace head
} // namespace leia
