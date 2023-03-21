#pragma once

#include "leia/sdk/helpers.hpp"

namespace leia {

namespace device { struct Config; }

namespace sdk {

enum class GraphicsAPI {
    OpenGL,
    D3D11,
    D3D12,
    Vulkan
};

struct Delegate;
struct LeiaSharpenParameters;
struct IThreadedInterlacer;
struct ThreadedInterlacerInitArgs;

class ML;

class ILeiaSDK
{
	friend class ThreadedInterlacer;
public:
    virtual void        Destroy                () = 0;
	virtual void        EnableSharpening       (bool enable) = 0;
	virtual float       GetCenterView          () const = 0;
	virtual int         GetDisplayWidth        () const = 0;
	virtual int         GetDisplayHeight       () const = 0;
	virtual int         GetMaxViews            () const = 0;
	virtual void        GetSharpenParameters   (LeiaSharpenParameters& sharpenParameters, int width, int height) const = 0;
	virtual int         GetViewHeight          () const = 0;
	virtual int         GetViewWidth           () const = 0;
	virtual bool		GetUseBilinearFiltering() const = 0;
	virtual bool        IsSharpeningEnabled    () const = 0;
	virtual void        SetCenterView          (float centerView) = 0;
	virtual void        Shutdown               () = 0;
	virtual void        Tick                   (float deltaTime) = 0;
	virtual void        ToggleBlendViews       () = 0;

	virtual void        SetConfig              (device::Config const&) = 0;
	virtual device::Config const& GetConfig    () = 0;

	virtual void        OnResume               () = 0;
	virtual void        OnPause                () = 0;

	virtual void        SetBacklight           (bool enable) = 0;
	virtual bool        GetBacklight           () = 0;

	virtual void        SetProfiling(bool enable) = 0;

	virtual void InitializeFaceTrackingInApp(bool enableFaceTracking, head::TrackingStateListener*, SharedCameraSink*) = 0;
	virtual void InitializeFaceTrackingInService(bool enableFaceTracking, head::TrackingStateListener*, leia_log_level serverLogLevel) = 0;
	virtual void SetFaceTrackingCallback(CLeiaSDKFaceTrackingCallback*) = 0;
	// Initialize/Release all face tracking related resources.
	// Consider using StartFaceTracking(false) if face tracking should be temporarily paused.
	// Do not call on the main thread to avoid stalls.
	virtual bool EnableFaceTracking(bool enable) = 0;
	virtual bool IsFaceTrackingEnabled() const = 0;
	// Start/Stop face tracking. Lightweight call - can be called on the main thread.
	virtual void StartFaceTracking(bool start) = 0;
	virtual bool IsFaceTrackingStarted() const = 0;
	// Do not call on the main thread to avoid stalls.
	virtual FaceDetectorConfig GetFaceTrackingConfig() const = 0;
	virtual void SetFaceTrackingConfig(FaceDetectorConfig) = 0;
	virtual bool IsFaceTrackingOnGpu() const = 0;
	virtual bool GetPrimaryFace(glm::vec3* position) const = 0;
	virtual bool GetNonPredictedPrimaryFace(glm::vec3* position) const = 0;
	virtual bool GetFaceTrackingProfiling(leia_headtracking_frame_profiling*) const = 0;
	virtual void SetFacePoint(head::MovingPoint const*, Timestamp const*) = 0;
	virtual bool IsFaceTrackingInFatalError(head::ServiceSpecificError* error, std::string* message) const = 0;

	virtual void InitializePlatform(PlatformInitArgs const&) = 0;
	virtual void Initialize(Delegate*) = 0;
	virtual bool IsInitialized() const = 0;

	virtual AssetManager* GetAssetManager() = 0;

	public:
	virtual IThreadedInterlacer* CreateNewThreadedInterlacer(ThreadedInterlacerInitArgs const&) = 0;
	virtual void Destroy(IThreadedInterlacer*) = 0;

    virtual ML* GetML() = 0;
	virtual ~ILeiaSDK() {};
};

// Single entry point for creating Leia SDK interface.
LEIASDK_API
ILeiaSDK* CreateLeiaSDK();

LEIASDK_API
bool IsFaceTrackingInService();

} // namespace sdk
} // namespace leia
