#pragma once

#include "leia/headTracking/engine/core/camera.hpp"
#include "leia/headTracking/engine/core/faceTracker.hpp"
#include "leia/headTracking/engine/core/faceDetector.hpp"

#include "leia/common/jniTypes.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <functional>

namespace leia {

class Platform;
class AssetManager;
struct SharedCameraSink;

#if defined(LEIA_OS_ANDROID)
namespace device { class SensorManager; }
#endif

namespace head {

struct BlinkDetectorConfiguration;
class AndroidCameraSinkGPU;
class AndroidCameraSinkCPU;
class FrameAdapter;

struct LightFilterConfiguration {
    bool enable = false;
    // Cut off threshold disables the face detector if ambient light is lower than the threshold.
    float cutoffLux = 500;
    // Check threshold defines the range in which faces should be constantly detected to output them in API.
    // It handles the transition from too low light (cut off), when no face is detected,
    // to enough light, when lighting conditions guarantee face detection.
    // In this range, it's expected for the face detector to detect a face unreliably.
    // So instead of outputting a flickering signal, we output it only if it's stable.
    float checkLux = 550;
    // Number of frames for which face must be detected to treat the signal as stable.
    int checkNumFrames = 90;
};

class LightFilter {
public:
    void SetConfiguration(LightFilterConfiguration);
    LightFilterConfiguration GetConfiguration() const { return _config; }

    bool ShouldDetectFaces(float lux);
    bool ShouldDropFaces(float lux, FaceDetector::Output const&);

private:
    LightFilterConfiguration _config;
    int _checkFramesCounter = 0;
};

struct EngineConfiguration {
    FaceTracker::Configuration faceTracker;
    FaceDetectorConfiguration faceDetector;
    CameraConfiguration camera;

    bool enableProfiling;

    LHT_ENGINE_API
    EngineConfiguration();
};

using EngineFrameCallback = std::function<void(FaceTracker const&, FaceDetector::Output const&, CameraFrame const&)>;

struct VirtualFace {
    glm::vec3 trackingPoint;
    float interocularDistance;
};
struct VirtualFaceHookContext {
    // Current timestamp.
    const Timestamp timestamp;
    // Current number of detected faces by the face detector.
    const int numDetectedFaces;
    // The maximum number of virtual faces that can be filled by the hook.
    const int maxNumVirtualFaces;
    // Buffer to be filled by virtual face hook.
    VirtualFace* const virtualFaces;
    // Actual number of virtual faces filled by the hook.
    int numVirtualFaces;
    // Whether to drop real detected faces or combine them with virtual faces.
    bool dropDetectedFaces;
};
// Returns true if the hook is active.
using VirtualFaceHook = std::function<bool(VirtualFaceHookContext* ctx)>;

struct EngineInitArgs {
    EngineConfiguration configuration;

    // Must be set
    Platform* platform = nullptr;

    // Optional. Engine will create its own asset manager if not provided.
    AssetManager* assetManager = nullptr;

    /// FrameCallback invoked after each processed CameraFrame.
    /// It's always called from the background thread where face detection happens.
    /// (Except the case when sharedCameraSink != nullptr. See SharedCameraSink for more info.)
    /// Therefore it's advised to keep FrameCallback as fast as possible.
    EngineFrameCallback frameCallback = nullptr;

    /// See FrameAdapter declaration for info.
    FrameAdapter* frameAdapter = nullptr;

    /// Allows managing the camera externally. See SharedCameraSink for more info.
    /// The user guarantees the validity of the pointer through the Engine's lifetime.
    SharedCameraSink* sharedCameraSink = nullptr;

    /// Use this to inject virtual detected face into the face tracking algorithm. Useful for debugging the multi-face case.
    VirtualFaceHook virtualFaceHook = nullptr;

    TrackingStateListener* trackingStateListener = nullptr;
};

struct TrackingSessionInitArgs {
    /// Android only.
    /// If set to a valid android.view.Surface object, android camera will use this as an additional output.
    /// Thus this surface should meet compatibility requirements:
    ///   * YUV or private format
    ///   * Size should be one of ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS
    /// Engine acquires strong reference to the object while tracking is active.
    /// Unused if the shared camera is in use.
    jobject androidSurface = nullptr;
};

/// Manages all head tracking components.
/// On Android, it's expected for the host application to request and handle camera permission.
/// Engine's API is not thread-safe.
class Engine final : public Camera::Listener {
public:
    LHT_ENGINE_API
    Engine(EngineInitArgs const&);
    LHT_ENGINE_API
    ~Engine();

    LHT_ENGINE_API
    bool StartTracking();
    LHT_ENGINE_API
    bool StartTracking(TrackingSessionInitArgs const&);
    LHT_ENGINE_API
    bool StopTracking() noexcept;

    LHT_ENGINE_API
    void SetTrackedEyes(bool left, bool right);
    LHT_ENGINE_API
    void SetSingleFaceConfig(SingleFaceConfiguration const&);

    /// Configures camera transform using Leia Display Config values.
    /// Can be overriden using SetCameraPosition, SetCameraRotation.
    LHT_ENGINE_API
    void SetCameraTransform(glm::vec3 const& cameraPosition, glm::vec3 const& cameraRotation);
    LHT_ENGINE_API
    void SetCameraPosition(glm::vec3 const& position);
    LHT_ENGINE_API
    void SetCameraRotation(
        float r00, float r01, float r02,
        float r10, float r11, float r12,
        float r20, float r21, float r22);
    LHT_ENGINE_API
    void SetFilterProperties(FilterProperties const&);

    LHT_ENGINE_API
    int GetMaxNumOfDetectedFacesLimit() const;
    LHT_ENGINE_API
    void SetMaxNumOfDetectedFaces(int);

    LHT_ENGINE_API
    bool SetFaceDetectorConfig(FaceDetectorConfig);
    LHT_ENGINE_API
    FaceDetectorConfig GetFaceDetectorConfig() const;

    LHT_ENGINE_API
    Slice<const int> GetSupportedCameraFps() const;
    LHT_ENGINE_API
    bool SetCameraFps(int fps);
    LHT_ENGINE_API
    int GetCameraFps() const;

    LHT_ENGINE_API
    void SetProfiling(bool enable);

    LHT_ENGINE_API
    float GetLux() const;
    LHT_ENGINE_API
    LightFilterConfiguration GetLightFilterConfiguration() const;
    LHT_ENGINE_API
    void SetLightFilterConfiguration(LightFilterConfiguration);

    LHT_ENGINE_API
    void SetHeadPoseZLowPassFilterAlpha(float alpha);
    LHT_ENGINE_API
    float GetHeadPoseZLowPassFilterAlpha() const;

private:
    enum ChangeTracker {
        Clean = 0,
        AllDirty = ~Clean,
        DirtyFaceTracker = 1 << 0,
        DirtyMaxNumOfFaces = 1 << 1,
    };

    void OnCameraFrameAvailable(Camera* camera, CameraFrame const& cameraFrame);
    void OnCameraIntrinsicsChange(Camera* camera, CameraIntrinsics const& intrinsics);

    bool OnFaceDetectorConfigChange();
    void UpdateFaceDetectorConfig();
    void UpdateCameraSink();

    void CreateCamera();
    void StartCameraCapture();

    void ProcessFrame(Camera*, CameraFrame const& cameraFrame);

    void CreateFaceDetector();

    float GetDeviceAcceleration();
    void PushDeviceAcceleration(glm::vec3 const&);

private:
    Platform* _platform;
    AssetManager* _assetManager;
    TrackingStateListener* _trackingStateListener;
    EngineConfiguration _config;
    EngineFrameCallback _frameCallback;
    FrameAdapter* _frameAdapter;

    VirtualFaceHook _virtualFaceHook;
    std::vector<VirtualFace> _virtualFacesBuffer;

    LightFilter _lightFilter;
    FaceDetector::Output _emptyFaceDetectorOutput;

    CameraIntrinsics _cameraIntrinsics;

#if defined(LEIA_OS_ANDROID)
    std::mutex _rollingMaxAccelerationMutex;
    float _lastAcceleration = 0.0f;
    float _rollingMaxAcceleration = 0.0f;
    int _numRollingMaxAccelerationSamples = 0;

    std::unique_ptr<device::SensorManager> _sensorManager;
#endif

    bool _isProfilingEnabled = false;
    std::atomic<bool> _trackingStarted;
    bool _notifyTrackingStart = false;

    std::mutex _configMutex;
    std::atomic<uint32_t> _dirtyBits = AllDirty;

    std::shared_ptr<FaceDetector> _faceDetector;
    std::unique_ptr<FaceTracker> _faceTracker;

#if defined(LEIA_OS_ANDROID)
    std::unique_ptr<AndroidCameraSinkCPU> _cpuSink;
    std::unique_ptr<AndroidCameraSinkGPU> _gpuSink;
#endif

    SharedCameraSink* _sharedCameraSink;
    std::atomic<bool> _isCameraValid;
    std::unique_ptr<Camera> _camera; // Camera should be the first to release, keep it at the bottom

    friend BlinkDetectorConfiguration GetBlinkDetectorConfiguration(Engine*);
};

LHT_ENGINE_API
bool IsCameraConnected();

LHT_ENGINE_API
uint32_t GetSupportedFaceDetectorBackends();

} // namespace head
} // namespace leia
