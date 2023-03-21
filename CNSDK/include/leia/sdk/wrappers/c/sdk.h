#pragma once

#include "leia/sdk/api.h"
#include "leia/common/log.h"
#include "leia/headTracking/common/frame.h"

BEGIN_CAPI_DECL

typedef struct leia_sdk_t { void*_; } * leia_sdk;

#pragma pack(push, 4)

struct leia_config
{
    float dotPitchInMM[2];
    int   panelResolution[2];
    int   numViews[2];
    int   sharpeningKernelXSize;
    float sharpeningKernelX[18];
    int   sharpeningKernelYSize;
    float sharpeningKernelY[18];
    int   viewResolution[2];
    int   displaySizeInMm[2];
    float act_gamma;
    float act_beta;
    float act_singleTapCoef;
    float systemDisparityPercent;
    float systemDisparityPixels;
    float cameraCenterX;
    float cameraCenterY;
    float cameraCenterZ;
    float cameraThetaX;
    float cameraThetaY;
    float cameraThetaZ;
    float centerViewNumber;
    float convergence;
    float n;
    float theta;
    float s;
    float d_over_n;
    float p_over_du;
    float p_over_dv;
    int   colorInversion;
    int   colorSlant;
    int   cameraWidth;
    int   cameraHeight;
    int   cameraFps;
    float cameraBinningFactor;
    float facePredictAlphaX;
    float facePredictAlphaY;
    float facePredictAlphaZ;
    float facePredictBeta;
    float facePredictLatencyMs;
    float accelerationThreshold;
    int   faceTrackingSingleFaceEnable;
    float faceTrackingSingleFaceTooFarDistanceThreshold;
    int   faceTrackingSingleFaceTooFarResetTimeoutMs;
    int   faceTrackingMaxNumOfFaces;
    float faceTrackingHeadPoseZLowPassAlpha;
};

#pragma pack(pop)

typedef int32_t leia_sdk_status;

enum // leia_sdk_status
{
    kLeiaSdkSuccess = 0,
    kLeiaSdkInvalidInstance,
    kLeiaSdkUnknownError,
};

LEIASDK_API
leia_sdk leiaSdkCreate(leia_log_level);

LEIASDK_API
leia_sdk_status leiaSdkSetFaceTrackingConfig(leia_sdk, leia_face_detector_config);

LEIASDK_API
leia_sdk_status leiaSdkEnableFacetracking(leia_sdk, leia_bool enable);

LEIASDK_API
leia_sdk_status leiaSdkStartFacetracking(leia_sdk, leia_bool start);

LEIASDK_API
leia_sdk_status leiaSdkIsFaceTrackingStarted(leia_sdk);

LEIASDK_API
leia_sdk_status leiaSdkSetProfiling(leia_sdk, leia_bool enable);

LEIASDK_API
leia_sdk_status leiaSdkGetFaceTrackingProfiling(leia_sdk, leia_headtracking_frame_profiling*);

LEIASDK_API
leia_sdk_status leiaSdkGetPrimaryFace(leia_sdk, leia_vector3_t* position);

LEIASDK_API
leia_sdk_status leiaSdkGetNonPredictedPrimaryFace(leia_sdk, leia_vector3_t* position);

LEIASDK_API
int leiaSdkGetConfig(leia_sdk, leia_config*);

LEIASDK_API
int leiaSdkSetConfig(leia_sdk, leia_config const* config);

LEIASDK_API
leia_sdk_status leiaSdkSetBacklight(leia_sdk, leia_bool enable);

LEIASDK_API
leia_sdk_status leiaSdkGetBacklight(leia_sdk, leia_bool* isEnabled);

LEIASDK_API
leia_sdk_status leiaSdkResume(leia_sdk);

LEIASDK_API
leia_sdk_status leiaSdkPause(leia_sdk);

LEIASDK_API
void leiaSdkShutdown(leia_sdk);

END_CAPI_DECL
