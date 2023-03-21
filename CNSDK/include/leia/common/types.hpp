#pragma once

#include <cstdint>

#include "leia/common/types.h"

namespace leia {

struct Timestamp {
    enum class Space: int {
        /// Timestamp measures the time since an unspecified starting point
        /// and is monotonically increasing.
        Unknown = -1,

        /// Timestamp measures the time in the same timebase as system clock.
        /// Different cameras match different time points:
        ///   * realsense - the middle of exposure
        ///   * android - the start of exposure
        /// Depending on platform different system clock is used:
        ///   * windows - std::chrono::system_clock::now
        ///   * android - android.os.SystemClock.elapsedRealtime
        System = 0,
    };
    Space space;

    double ms;
};

struct ImageDesc {
    int32_t width;
    int32_t height;
    int32_t rotation;
    uint8_t const* data;
};

struct CameraIntrinsics {
    int width; /* Width of the image in pixels */
    int height; /* Height of the image in pixels */
    float ppx; /* Horizontal coordinate of the principal point of the image, as a pixel offset from the left edge */
    float ppy; /* Vertical coordinate of the principal point of the image, as a pixel offset from the top edge */
    float fx; /* Focal length of the image plane, as a multiple of pixel width */
    float fy; /* Focal length of the image plane, as a multiple of pixel height */
    float distortionCoeffs[5]; /* Distortion coefficients */
    bool isMirrored; /* Whether the image is mirrored or not */
};

using FaceDetectorBackend = leia_face_detector_backend;
using FaceDetectorInputType = leia_face_detector_input_type;
using FaceDetectorConfig = leia_face_detector_config;

const char* ToStr(FaceDetectorBackend);
const char* ToUiStr(FaceDetectorBackend);
const char* ToStr(FaceDetectorInputType);
const char* ToUiStr(FaceDetectorInputType);

using SourceLocation = leia_source_location;

} // namespace leia
