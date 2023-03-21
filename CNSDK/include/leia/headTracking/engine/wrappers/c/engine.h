#pragma once

#include "leia/common/log.h"
#include "leia/headTracking/engine/api.h"
#include "leia/headTracking/common/frame.h"

BEGIN_CAPI_DECL

typedef struct leia_headtracking_engine_t { void*_; } * leia_headtracking_engine;

#pragma pack(push, 4)

typedef struct leia_headtracking_kalman_coeffs_t {
    leia_vector3 a;
    leia_vector3 b;
} leia_headtracking_kalman_coeffs;

typedef struct leia_headtracking_filter_properties_t {
    float accelerationThreshold;
    float survivalTimeMs;
    float velDampingTime;
    float minFitDist;
    float maxFitDist;
    float angleYMax;
    float interocularDistanceMin;
    float interocularDistanceMax;
    float bufferLim;
    leia_vector3 mainFaceSkew;
    leia_vector3 mainFaceScale;
    leia_headtracking_kalman_coeffs kalman;
} leia_headtracking_filter_properties;

typedef struct leia_headtracking_virtual_face_t {
    leia_vector3 trackingPoint;
    float interocularDistance;
} leia_headtracking_virtual_face;

typedef struct leia_headtracking_virtual_hook_context_t {
    const leia_timestamp timestamp;
    const int32_t numDetectedFaces;
    const int32_t maxNumVirtualFaces;
    leia_headtracking_virtual_face* const virtualFaces;
    int32_t numVirtualFaces;
    leia_bool dropDetectedFaces;
} leia_headtracking_virtual_hook_context;

// Expected to return a number of virtual faces filled in the output.
typedef leia_bool(*leia_headtracking_virtual_face_hook_t)(leia_headtracking_virtual_hook_context*, void* user_data);

typedef struct leia_headtracking_engine_init_args_t {
    int32_t cameraWidth;
    int32_t cameraHeight;
    int32_t cameraFps;
    float cameraBinningFactor;

    int32_t detectorMaxNumOfFaces;

    leia_log_level logLevel;

    leia_headtracking_filter_properties filterProperties;

    leia_headtracking_frame_listener_t frameListener;
    void* frameListenerUserData;

    leia_bool enablePolling;

    leia_headtracking_virtual_face_hook_t virtualFaceHook;
    void* virtualFaceHookUserData;

    // TODO: add shared camera
} leia_headtracking_engine_init_args;

#pragma pack(pop)

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineInitArgs(leia_headtracking_engine_init_args const* init_args, leia_headtracking_engine* out_engine);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineStartTracking(leia_headtracking_engine);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineStopTracking(leia_headtracking_engine);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineSetTrackedEyes(leia_headtracking_engine, leia_bool left_eye, leia_bool right_eye);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineSetCameraTransform(leia_headtracking_engine, leia_vector3_t cameraPosition, leia_vector3_t cameraRotation);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineSetCameraPosition(leia_headtracking_engine, float x, float y, float z);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineSetCameraRotation(leia_headtracking_engine,
    float r00, float r01, float r02,
    float r10, float r11, float r12,
    float r20, float r21, float r22);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineSetFilterProperties(leia_headtracking_engine, leia_headtracking_filter_properties const*);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineGetMaxNumOfDetectedFacesLimit(leia_headtracking_engine, int32_t* out);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineSetMaxNumOfDetectedFaces(leia_headtracking_engine, int32_t maxNumOfDetectedFaces);

LHT_ENGINE_API
leia_headtracking_status
leiaHeadTrackingEngineGetLatestFrame(leia_headtracking_engine, leia_headtracking_frame*);

LHT_ENGINE_API
void leiaHeadTrackingEngineRelease(leia_headtracking_engine);

END_CAPI_DECL
