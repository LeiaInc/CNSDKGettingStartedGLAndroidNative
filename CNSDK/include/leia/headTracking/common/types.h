#pragma once

#include "leia/common/types.h"

BEGIN_CAPI_DECL

typedef int32_t leia_headtracking_status;

enum // leia_headtracking_status
{
    kLeiaHeadTrackingStatusSuccess = 0,
    kLeiaHeadTrackingStatusErrorInvalidInstance,
    kLeiaHeadTrackingStatusErrorUnknown,
};

#pragma pack(push, 4)

typedef leia_vector3 leia_headtracking_point;

typedef struct leia_headtracking_moving_point_t {
    leia_vector3 pos;
    leia_vector3 vel;
} leia_headtracking_moving_point;

typedef uint32_t leia_headtracking_face_idx;

typedef enum leia_headtracking_eye_idx_e {
    kLeiaHeadTrackingRightEyeIdx = 0,
    kLeiaHeadTrackingLeftEyeIdx,
    kLeiaHeadTrackingNumEyes
} leia_headtracking_eye_idx;

typedef struct leia_headtracking_face_t {
    // Tracking point - 3D coordinate with camera transform and Kalman filter applied.
    leia_headtracking_moving_point point;
    // Pose angle
    leia_vector3 angle;
    // // Index into the raw faces array.
    leia_headtracking_face_idx rawFaceIndex;
} leia_headtracking_face;

typedef struct leia_headtracking_raw_face_t {
    // Deprojected camera-space eye position.
    leia_vector3 eyePoints[kLeiaHeadTrackingNumEyes];
    // The point of tracking, see Configuration::trackRightEye/Configuration::trackLeftEye.
    leia_vector3 trackingPoint;
    // Index into FaceDetector::Output::faces.
    leia_headtracking_face_idx detectedFaceIndex;
} leia_headtracking_raw_face;

typedef struct leia_headtracking_detected_face_eye_t {
    // 2D coordinate on camera frame image plane (top-left origin)
    leia_vector2d imageCoord;
    // Z component in camera-space (projected distance)
    float depth;
} leia_headtracking_detected_face_eye;

typedef struct leia_headtracking_detected_face_t {
    leia_headtracking_detected_face_eye eyes[kLeiaHeadTrackingNumEyes];

    // Head location in mm. The origin point is the location of the camera.
    leia_vector3 posePosition;
    // Head rotation in radians. The rotation is a left handed coordinate system.
    leia_vector3 poseAngle;
    uint32_t id;
} leia_headtracking_detected_face;

#define LEIA_HEADTRACKING_MAX_NUM_FACES 3

typedef struct leia_headtracking_tracking_result_t {
    int32_t num_faces;
    leia_headtracking_face faces[LEIA_HEADTRACKING_MAX_NUM_FACES];

    leia_timestamp timestamp;

    int32_t jumpFlag;
} leia_headtracking_tracking_result;

#pragma pack(pop)

END_CAPI_DECL
