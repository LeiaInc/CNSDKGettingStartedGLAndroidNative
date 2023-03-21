#pragma once

#include "leia/headTracking/engine/core/faceDetector.hpp"
#include "leia/headTracking/engine/core/kalmanFilter.hpp"
#include "leia/headTracking/engine/core/camera.hpp"
#include "leia/headTracking/common/types.hpp"

#include <vector>
#include <chrono>

namespace leia {
namespace head {

class LowPassFilter {
public:
    LowPassFilter();

    void Reconfigure(float alpha);
    bool IsValid() const;
    float GetAlpha() const { return _alpha; };

    float Update(float value);

private:
    float _alpha;
    float _previousValue;
};

struct FilterProperties {
    float accelerationThreshold = 0.5f;
    float survivalTimeMs = 600.0f;
    float velDampingTime = 100.0f;
    float minFitDist = 40.0f;
    float maxFitDist = 2000.0f;
    float angleYMax = glm::radians(45.0f);
    float interocularDistanceMin = 30.0f;
    float interocularDistanceMax = 80.0f;
    float bufferLim = 600.0f;
    glm::vec3 mainFaceSkew = glm::vec3(0.0f);
    glm::vec3 mainFaceScale = glm::vec3(4.0f, 4.0f, 1.0f);
    KalmanCoeffs kalman;
};

/// Matches faces between frames. Processes detected faces:
/// converts screen coordinates to camera space and applies Kalman filter to it.
class FaceTracker {
public:
    struct Configuration {
        /// At least one of eyes should be tracked, i.e. (!trackRightEye && !trackLeftEye) is invalid.
        bool trackRightEye = true;
        bool trackLeftEye = true;

        FilterProperties filterProperties;

        glm::mat3 cameraRotation = glm::mat3(1.0f);
        glm::vec3 cameraPosition = glm::vec3(0.0f);

        SingleFaceConfiguration singleFace;

        float headPoseZLowPassAlpha = 0.2f;
    };

    LHT_ENGINE_API
    FaceTracker();

    /// Must be called whenever camera parameters, such as intrinsics, change.
    LHT_ENGINE_API
    void CameraDidChange(CameraIntrinsics const& intrinsics);

    /// Must be called whenever the max number of detected faces change.
    LHT_ENGINE_API
    void OnMaxNumOfDetectedFacesChange(int newMaxNumOfFaces);

    /// Process detected faces \p faceDetectorOutput on \p frame. After this call,
    /// \ref faces and \ref timestamp return values corresponding to the current frame.
    LHT_ENGINE_API
    void Update(FaceDetector*, FaceDetector::Output const& faceDetectorOutput, CameraFrame const& frame, float deviceAcceleration);

    /// Invalid configuration is ignored.
    LHT_ENGINE_API
    void UpdateConfiguration(Configuration const& configuration);

    LHT_ENGINE_API
    Slice<const RawFace> rawFaces() const;

    /// The most recent detected faces. See \ref Update.
    LHT_ENGINE_API
    Slice<const Face> faces() const;

    int jumpFlag() const { return _jumpFlag; }

    /// The most recent timestamp. See \ref Update.
    Timestamp timestamp() const { return _timestamp; }

    KalmanCoeffs const& kalmanCoeffs() const { return _config.filterProperties.kalman; }
    CameraIntrinsics const& cameraIntrinsics() const { return _cameraIntrinsics; }
    Configuration const& config() const { return _config; }

    LowPassFilter* GetHeadPoseZFilter() { return &_headPoseZFilter; }

private:
    void Reset();
    bool IsValid(Configuration const& config);
    MovingPoint ApplyCameraTransform(MovingPoint const&);
    glm::vec3 ApplyCameraTransform(glm::vec3 const&, bool position);
    bool ConvertEyeCenterToPoint(
        CameraFrame const& frame,
        ImageDesc const& frameDesc,
        DetectedFace::Eye const& eye,
        glm::vec3* eyePoint);
    FaceIdx UpdateSingleFaceTracking(FaceDetector::Output const&);

private:
    Configuration _config;
    CameraIntrinsics _cameraIntrinsics;

    int _previousOrientation = -1;

    Timestamp _timestamp;
    std::vector<RawFace> _rawFaces;

    template <typename T>
    class DoubleBuffer {
    public:
        T& GetAndSwap() {
            int currentBufferIdx = _bufferIdx;
            _bufferIdx = (_bufferIdx + 1) % kBufferSize;
            return _buffer[currentBufferIdx];
        }

        T& Get() {
            return _buffer[_bufferIdx];
        }

        T const& Get() const {
            return _buffer[_bufferIdx];
        }

    private:
        int _bufferIdx = 0;
        static const int kBufferSize = 2;
        T _buffer[kBufferSize];
    };

    // Kalman stage
    DoubleBuffer<std::vector<KalmanFilter>> _filters;
    std::vector<FaceIdx> _filterToRawFaceIdx;

    // Transform stage
    std::vector<MovingPoint> _transformedPoints;
    
    // Result stage
    int _jumpFlag = 0;
    float _buffer = 0.0f;
    DoubleBuffer<std::vector<Face>> _resultFaces;

    // Single face tracking auto-reset
    struct Timer {
        std::chrono::steady_clock::time_point startTime;
        std::chrono::duration<double> duration;

        Timer() = default;
        Timer(std::chrono::steady_clock::time_point now, std::chrono::duration<double> duration)
            : startTime(now)
            , duration(duration) {

        }

        bool IsStarted() const { return startTime.time_since_epoch().count() != 0; }
        bool IsExpired(std::chrono::steady_clock::time_point now) const { return now - startTime > duration; }
    };
    Timer _singleFaceTrackingTimer;

    LowPassFilter _headPoseZFilter;
};

} // namespace head
} // namespace leia
