#pragma once

#include "leia/headTracking/engine/core/camera.hpp"

#include <functional>
#include <atomic>

namespace leia {

struct SharedCameraSink;

namespace head {

/// SharedCamera allows a user to manage the camera.
///
/// All of the face tracking is driven by SharedCameraSink::imageCallback.
/// Meaning face detection and face tracking happens in SharedCameraSink::imageCallback.
/// EngineFrameCallback/FrameAdapter::Callback is invoked right at the end of SharedCameraSink::imageCallback.
class SharedCamera : public Camera {
public:
    LHT_ENGINE_API
    SharedCamera(Listener* listener, SharedCameraSink* sharedCameraSink);
    LHT_ENGINE_API
    ~SharedCamera() override;

    void StartCapture(CameraCaptureConfiguration const&) override {};
    void StopCapture() override {};

    int GetFps() const override { return 0; }
    bool SetFps(int) override { return false; }
    Slice<const int> GetSupportedFps() const override { return {}; }

    float GetLux() const override { return _lux; }

private:
    Listener* _listener;
    SharedCameraSink* _sharedCameraSink;
    std::atomic<float> _lux = kInvalidLux;
};

} // namespace head
} // namespace leia
