#pragma once

#include "leia/headTracking/engine/core/camera.hpp"

namespace leia::head {

/// A part of full realsense camera configuration that can be changed after camera creation, hence the name.
struct RealsenseCameraDynamicConfiguration {
    /// RS2_OPTION_EMITTER_ENABLED
    bool enableEmitter = false;
    /// RS2_OPTION_ENABLE_AUTO_EXPOSURE
    /// In case enableAutoExposure is true, exposure and gain parameters are not used.
    bool enableAutoExposure = true;
    /// RS2_OPTION_EXPOSURE
    float exposure = 9000.0f;
    /// RS2_OPTION_GAIN
    float gain = 50.0f;
};

struct RealsenseCameraConfiguration {
    CameraConfiguration common;
    RealsenseCameraDynamicConfiguration dynamic;
};

/// Attempts to start a streaming pipeline with the provided configuration. In case of error, throws an exception.
LHT_ENGINE_API
std::unique_ptr<Camera> CreateRealsenseCamera(Camera::Listener* listener, RealsenseCameraConfiguration const& configuration);

LHT_ENGINE_API
void UpdateRealsenseCameraDynamicConfiguration(Camera* camera, RealsenseCameraDynamicConfiguration const& configuration);

LHT_ENGINE_API
bool IsRealsenseCameraConnected();

} // namespace leia::head
