#pragma once

#include "leia/sdk/api.h"

#include <stdint.h>

BEGIN_CAPI_DECL

enum leia_sdk_image_component_type {
    kLeiaSdkImageComponentUint8 = 0,
    kLeiaSdkImageComponentFloat,
};

enum leia_sdk_image_format {
    kLeiaSdkImageFormatRGB = 0,
    kLeiaSdkImageFormatRGBA,
    kLeiaSdkImageFormatBGR,
    kLeiaSdkImageFormatBGRA,
    kLeiaSdkImageFormatR,
};

struct leia_sdk_image_desc {
    uint8_t const* data;
    uint32_t width;
    uint32_t height;
    uint32_t rowBytes;
    leia_sdk_image_format format;
    leia_sdk_image_component_type componentType;
};

LEIASDK_API
uint32_t leia_sdk_image_component_type_size(leia_sdk_image_component_type);
LEIASDK_API
uint32_t leia_sdk_image_format_num_components(leia_sdk_image_format);

#ifdef LEIA_USE_OPENGL
LEIASDK_API
uint32_t leia_sdk_image_format_to_gl(leia_sdk_image_format);
LEIASDK_API
uint32_t leia_sdk_image_component_type_to_gl(leia_sdk_image_component_type);
#endif // LEIA_USE_OPENGL

END_CAPI_DECL
