#pragma once

#include "leia/headTracking/engine/api.h"

namespace leia {
namespace head {

class CameraFrame;

LHT_ENGINE_API
bool SaveCameraFrame(CameraFrame const& cameraFrame, const char* path);

} // namespace head
} // namespace leia
