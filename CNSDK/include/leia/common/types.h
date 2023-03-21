#pragma once

#include "leia/common/defines.h"

#include <stdint.h>

BEGIN_CAPI_DECL

typedef int32_t leia_bool;

#pragma pack(push, 4)

typedef enum leia_timestamp_space_e {
    kLeiaTimestampSpaceUnknown = -1,
    kLeiaTimestampSpaceSystem = 0,
} leia_timestamp_space;

typedef struct leia_timestamp_t {
    leia_timestamp_space space;
    double ms;
} leia_timestamp;

typedef struct leia_vector2d_t {
    double x;
    double y;
} leia_vector2d;

typedef struct leia_vector3_t {
    float x;
    float y;
    float z;
} leia_vector3;

typedef enum leia_face_detector_backend_e {
    kLeiaFaceDetectorBackendUnknown = 0,
    kLeiaFaceDetectorBackendCPU = 1 << 0,
    kLeiaFaceDetectorBackendGPU = 1 << 1,

    kNumLeiaFaceDetectorBackends = 2,
} leia_face_detector_backend;

typedef enum leia_face_detector_input_type_e {
    kLeiaFaceDetectorInputTypeUnknown = 0,
    kLeiaFaceDetectorInputTypeCPU = 1 << 0,
    kLeiaFaceDetectorInputTypeGPU = 1 << 1,

    kNumLeiaFaceDetectorInputTypes = 2,
} leia_face_detector_input_type;

typedef struct leia_face_detector_config_s {
    leia_face_detector_backend backend;
    leia_face_detector_input_type inputType;
} leia_face_detector_config;

typedef struct leia_source_location_s {
    const char* filename;
    const char* funcname;
    int line;
} leia_source_location;

#ifndef LEIA_FUNCTION
# define LEIA_FUNCTION __FUNCTION__
#endif

#define LEIA_SOURCE_LOCATION leia_source_location{__FILE__, LEIA_FUNCTION, __LINE__}

#pragma pack(pop)

END_CAPI_DECL
