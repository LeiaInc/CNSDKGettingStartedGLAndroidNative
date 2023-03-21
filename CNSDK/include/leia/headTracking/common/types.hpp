#pragma once

#include "leia/common/types.hpp"
#include "leia/headTracking/common/types.h"

#include <glm/glm.hpp>

#include <cstdint>

namespace leia {
namespace head {

using Point = glm::vec3;

struct MovingPoint {
    glm::vec3 pos;
    glm::vec3 vel;
};

using FaceIdx = uint32_t;
const FaceIdx kInvalidFaceIdx = std::numeric_limits<uint32_t>::max();

enum EyeIdx {
    kRightEyeIdx = 0,
    kLeftEyeIdx,
    kNumEyes
};

struct Face {
    // Tracking point - 3D coordinate with camera transform and Kalman filter applied.
    MovingPoint point;
    // Pose angle.
    glm::vec3 angle;
    // Index into the raw faces array.
    FaceIdx rawFaceIndex;
};

struct RawFace {
    // Deprojected camera-space eye position.
    glm::vec3 eyePoints[kNumEyes];
    // The point of tracking, see Configuration::trackRightEye/Configuration::trackLeftEye.
    glm::vec3 trackingPoint;
    // Index into FaceDetector::Output::faces.
    FaceIdx detectedFaceIndex;
};

struct DetectedFace {
    struct Eye {
        // 2D coordinate on camera frame image plane (top-left origin)
        glm::dvec2 imageCoord;
        // Z component in camera-space (projected distance)
        float depth;
    };
    Eye eyes[kNumEyes];

    // Head location in mm. The origin point is the location of the camera.
    glm::vec3 posePosition;
    // Head rotation in radians. The rotation is a left handed coordinate system.
    glm::vec3 poseAngle;

    FaceIdx id;
};

struct Imu {
    float acceleration;
};

struct TrackingStateListener {
    // isStarted=true: called right before the first frame arrives.
    // isStarted=false: called after the last frame arrives.
    virtual void OnTrackingStatusUpdate(bool isStarted) = 0;
};

struct FaceTrackingStatus {
    bool isStarted;
};
struct FaceTrackingProfilingStatus {
    bool isEnabled;
};
struct FaceDetectorMaxNumOfFaces {
    int value;
};

// Single-face mode forces tracking of a single face.
// This allows the face detector to disable the detection stage,
// and do only the tracking stage. It improves overall face tracking performance.
//
// Additionally, this mode automatically switches between available faces,
// if the currently tracked one stays too far for some period of time.
//
struct SingleFaceConfiguration {
    bool enable = false;
    float tooFarDistanceThreshold = 0.8f;
    int tooFarResetTimeoutMs = 1000;
};
    
} // namespace head
} // namespace leia
