#pragma once

#include "leia/headTracking/engine/api.h"
#include "leia/headTracking/common/types.hpp"
#include "leia/common/slice.hpp"

#include <memory>
#include <functional>

namespace leia {

struct CameraIntrinsics;

namespace head {

/// CameraFrame stores single-frame data.
/// It may be a stereo frame (ImageDescs().length == 2) or mono frame.
/// It may provide depth information (CameraFrame::GetDepth).
class CameraFrame {
public:
    LHT_ENGINE_API
    virtual ~CameraFrame();

    /// Returns timestamp of the camera frame.
    virtual Timestamp timestamp() const = 0;

    /// Returns the whole description of images contained in the frame.
    /// Returns 2 images in case of stereo camera.
    /// Valid only for a lifetime of the frame.
    virtual Slice<const ImageDesc> imageDescs() const = 0;

    /// Camera space Z-component, don't confuse with distance from a camera.
    virtual bool GetDepth(glm::dvec2 pixel, float* depth) const = 0;

    /// Computes the corresponding point in 3D camera space.
    virtual bool DeprojectPixelToPoint(glm::dvec2 pixel, float depth, glm::vec3* point) const = 0;

    /// Computes the corresponding pixel coordinate and depth.
    virtual bool ProjectPointToPixel(glm::vec3 const&, glm::dvec2*, float*) const = 0;

    virtual bool GetGpuTextureId(uint32_t*) const { return false; }
};

struct CameraConfiguration {
    /// Keep zeros to use automatic/default values
    int width = 0;
    int height = 0;
    int fps = 0;
    float binningFactor = 0.0f;
};

struct CameraCaptureConfiguration {
    bool enableLux = false;
};

using CameraFrameCallback = std::function<void(std::unique_ptr<CameraFrame>)>;

/// Provides CameraFrame using either polling mechanism (Camera::WaitForFrame) or event-based (Camera::Listener)
class Camera {
public:
    LHT_ENGINE_API
    virtual ~Camera();

    struct Listener {
        virtual void OnCameraFrameAvailable(Camera*, CameraFrame const&) = 0;
        virtual void OnCameraIntrinsicsChange(Camera*, CameraIntrinsics const&) = 0;
    };

    virtual void StartCapture(CameraCaptureConfiguration const&) = 0;

    virtual void StopCapture() = 0;

    virtual int GetFps() const = 0;
    virtual bool SetFps(int fps) = 0;
    virtual Slice<const int> GetSupportedFps() const = 0;

    virtual float GetLux() const = 0;
};

const float kInvalidLux = 0.0f;

} // namespace head
} // namespace leia
