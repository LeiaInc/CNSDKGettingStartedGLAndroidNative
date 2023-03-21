#pragma once

#include "leia/headTracking/engine/api.h"
#include "leia/headTracking/engine/core/camera.hpp"
#include "leia/headTracking/common/types.hpp"

#include <vector>

namespace leia {
namespace head {

class LowPassFilter;

struct FaceDetectorConfiguration {
    // Max number of faces to be detected at the same time, note that the limit for this number depends on a concrete FaceDetector implementation.
    // Set to zero to use the max possible number.
    int maxNumOfFaces = 0;
    FaceDetectorBackend backend = kLeiaFaceDetectorBackendUnknown;
    FaceDetectorInputType inputType = kLeiaFaceDetectorInputTypeUnknown;

    LowPassFilter* headPoseZFilter = nullptr;
};

/// Sole purpose of FaceDetector is to detect faces on CameraFrame.
/// Each DetectedFace consists of two eyes each of which has pixel coordinate
/// on CameraFrame and its real-world depth (camera space Z-component, not distance from camera).
class FaceDetector {
public:
    virtual ~FaceDetector() = default;

    virtual int GetMaxNumOfFacesLimit() const = 0;

    virtual FaceDetectorConfiguration const& GetConfig() const = 0;

    /// Must be called whenever camera parameters, such as intrinsics, change.
    virtual void CameraDidChange(CameraIntrinsics const& intrinsics) = 0;

    struct Output {
        std::vector<DetectedFace> faces;
    };

    virtual Output& DetectFaces(CameraFrame const& frame) = 0;

    virtual bool TrackSingleFace(FaceIdx id) = 0;
};

} // namespace head
} // namespace leia
