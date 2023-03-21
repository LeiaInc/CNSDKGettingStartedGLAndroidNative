#pragma once

#include "leia/headTracking/common/api.h"
#include "leia/headTracking/common/types.h"

BEGIN_CAPI_DECL

typedef struct leia_headtracking_frame_t { void*_; } * leia_headtracking_frame;
typedef void (*leia_headtracking_frame_listener_t)(leia_headtracking_frame, void* user_data);

#pragma pack(push, 4)

typedef struct leia_headtracking_raw_faces_t {
    int numFaces;
    leia_headtracking_raw_face faces[LEIA_HEADTRACKING_MAX_NUM_FACES];
} leia_headtracking_raw_faces;

typedef struct leia_headtracking_detected_faces_t {
    int numFaces;
    leia_headtracking_detected_face faces[LEIA_HEADTRACKING_MAX_NUM_FACES];
} leia_headtracking_detected_faces;

// All timestamps are in the system clock space. See Platform::GetSystemTimeNs.
typedef struct leia_headtracking_frame_profiling_t {
    int64_t cameraExposureTime;
    // Face detector starts processing a camera frame
    int64_t faceDetectorStartTime;
    // Face detector ends processing a camera frame
    int64_t faceDetectorEndTime;
    // Engine sends frame to the user
    int64_t apiTimestamp;
} leia_headtracking_frame_profiling;

#pragma pack(pop)

LHT_COMMON_API
leia_headtracking_status
leiaHeadTrackingFrameGetTrackingResult(leia_headtracking_frame, leia_headtracking_tracking_result*);

LHT_COMMON_API
leia_headtracking_status
leiaHeadTrackingFrameGetRawFaces(leia_headtracking_frame, leia_headtracking_raw_faces*);

LHT_COMMON_API
leia_headtracking_status
leiaHeadTrackingFrameGetDetectedFaces(leia_headtracking_frame, leia_headtracking_detected_faces*);

LHT_COMMON_API
leia_headtracking_status
leiaHeadTrackingFrameGetProfiling(leia_headtracking_frame, leia_headtracking_frame_profiling*);

LHT_COMMON_API
void
leiaHeadTrackingFrameRelease(leia_headtracking_frame);

END_CAPI_DECL
