#pragma once

#include "leia/device/api.h"

#include <glm/glm.hpp>

#include <functional>

namespace leia { class Platform; }

namespace leia::device {

struct Config
{
	glm::vec2 dotPitchInMM       = { 0.0f, 0.0f };
	glm::ivec2 panelResolution   = { 0, 0 };
	glm::ivec2 numViews          = { 0, 0 };
	int   sharpeningKernelXSize  = 0;
	float sharpeningKernelX[18]  = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	int   sharpeningKernelYSize  = 0;
	float sharpeningKernelY[18]  = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	glm::ivec2 viewResolution    = { 0, 0 };
	glm::ivec2 displaySizeInMm   = { 0, 0 };
	float act_gamma              = 0.0f;
	float act_beta               = 0.0f;
	float act_singleTapCoef      = 0.12f;
	float systemDisparityPercent = 0.0f;
	float systemDisparityPixels  = 0.0f;
	float cameraCenterX          = 0.0f;
	float cameraCenterY          = 0.0f;
	float cameraCenterZ          = 0.0f;
	float cameraThetaX           = 0.0f;
	float cameraThetaY           = 0.0f;
	float cameraThetaZ           = 0.0f;
	float centerViewNumber       = 0.0f;
	float convergence            = 0.0f;
	float n                      = 0.0f;
	float theta                  = 0.0f;
	float s                      = 0.0f;
	float d_over_n               = 0.0f;
	float p_over_du              = 0.0f;
	float p_over_dv              = 0.0f;
	bool  colorInversion         = false;
	int   colorSlant             = 0;
	int   cameraWidth            = 0;
	int   cameraHeight           = 0;
	int   cameraFps              = 0;
	float cameraBinningFactor    = 0;
    float facePredictAlphaX      = 0.02f;
    float facePredictAlphaY      = 0.05f;
    float facePredictAlphaZ      = 0.2f;
    float facePredictBeta        = 0.1f;
    float facePredictLatencyMs   = 40.0f;
    float accelerationThreshold  = 0.6f;

    bool faceTrackingSingleFaceEnable = false;
    float faceTrackingSingleFaceTooFarDistanceThreshold = 0.8f;
    int faceTrackingSingleFaceTooFarResetTimeoutMs = 1000;
    int faceTrackingMaxNumOfFaces = 0;

    float faceTrackingHeadPoseZLowPassAlpha = 0.2f;

	void debugLog();
};

using CompletionCallback = std::function<void(bool success)>;

LEIADEVICE_API
bool Connect(Platform*);

LEIADEVICE_API
bool InitBacklightController(CompletionCallback);
LEIADEVICE_API
void ShutdownBacklightController();
LEIADEVICE_API
void SetBacklightMode(bool enableLightfield);
LEIADEVICE_API
bool GetBacklightMode();

/// Start potentially asynchronous request to get the display config.
/// CompletionCallback might be called from a background thread thus it should be thread-safe.
/// It's guaranteed that \p config is initialized at the time of \r CompletionCallback call and it is
/// either filled with actual values from the device or is filled with default values.
/// Each call to \r RequestConfig that returns true must be accompanied by a call to \r CancelConfigRequest.
/// The pointer to the config must stay valid until request is cancelled through \r CancelConfigRequest.
LEIADEVICE_API
bool RequestConfig(Config* config, CompletionCallback);
/// Cancel asynchronous request launched by \r RequestConfig.
/// It's still possible that the request will be fulfilled while this function is called.
/// Safe to be called even if there was no \r RequestConfig call or multiple times.
LEIADEVICE_API
void CancelConfigRequest();

} // namespace leia::device
