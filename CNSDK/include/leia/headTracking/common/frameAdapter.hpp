#pragma once

#include "leia/headTracking/common/api.h"
#include "leia/headTracking/common/frame.h"
#include "leia/headTracking/common/types.hpp"
#include "leia/common/slice.vector.hpp"

#include <memory>
#include <mutex>
#include <functional>

namespace leia {
namespace head {

// TODO: provide only the requested content
enum FrameContent {
    kFrameTrackingContent = 1 << 0,
    kFrameRawContent = 1 << 1,
    kFrameDetectedContent = 1 << 2,
    kFrameProfilingContent = 1 << 3,
};

struct Frame {
    // Tracking
    virtual Slice<Face const> GetFaces() const = 0;
    virtual Timestamp GetTimestamp() const = 0;
    virtual bool GetJumpFlag() const = 0;
    virtual Imu GetImu() const = 0;

    // Raw
    virtual Slice<RawFace const> GetRawFaces() const = 0;

    // Detected
    virtual Slice<DetectedFace const> GetDetectedFaces() const = 0;

    // Profiling
    virtual leia_headtracking_frame_profiling GetProfiling() const = 0;

protected:
    // Should never be released through base class
    LHT_COMMON_API
    ~Frame();
};

struct BufferedFrame : public Frame {
    std::vector<Face> faces;
    Timestamp timestamp;
    bool jumpFlag;
    Imu imu;

    std::vector<RawFace> rawFaces;

    std::vector<DetectedFace> detectedFaces;

    leia_headtracking_frame_profiling_t profiling;

    LHT_COMMON_API
    BufferedFrame();
    LHT_COMMON_API
    BufferedFrame(Frame const& frame);

    Slice<Face const> GetFaces() const override { return ToSlice(faces); }
    Timestamp GetTimestamp() const override { return timestamp; }
    bool GetJumpFlag() const override { return jumpFlag; }
    Imu GetImu() const override { return imu; }

    Slice<RawFace const> GetRawFaces() const override { return ToSlice(rawFaces); }

    Slice<DetectedFace const> GetDetectedFaces() const override { return ToSlice(detectedFaces); }

    leia_headtracking_frame_profiling_t GetProfiling() const override { return profiling; };
};

/// Allows using polling approach for getting per frame data instead of relying on asynchronous frameCallback.
/// Unifies frame data access API between service and in-app implementations.
class FrameAdapter {
public:
    // If provided, called with every new Frame
    using Callback = std::function<void(Frame const&)>;

    LHT_COMMON_API
    FrameAdapter(Callback callback, bool enablePolling);

    // Polling API - should be enabled explicitly, otherwise throws an exception.
    LHT_COMMON_API
    BufferedFrame GetFrame() const;

    // Frame provider API, e.g. Engine or Service
    void OnFrame(Frame const&);

private:
    Callback _callback;

    class Poller {
    public:
        BufferedFrame GetFrame();
        void OnFrame(Frame const&);

    private:
        static const int kBufferSize = 2;
        int _bufferIdx = 0;
        BufferedFrame _buffer[kBufferSize] = {};
        std::mutex _bufferMutex;
    };
    std::unique_ptr<Poller> _poller;
};

struct WrappedFrame {
    virtual ~WrappedFrame();
    virtual Frame const& GetFrame() const = 0;
};

LHT_COMMON_API
std::unique_ptr<WrappedFrame> WrapFrame(Frame const& frame);
LHT_COMMON_API
std::unique_ptr<WrappedFrame> WrapFrame(BufferedFrame&& bufferedFrame);

} // namespace head
} // namespace leia
