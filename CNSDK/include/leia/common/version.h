#pragma once

#include "leia/common/api.h"

#define CNSDK_VERSION "0.6.143"
#define CNSDK_MAJOR_VERSION 0
#define CNSDK_MINOR_VERSION 6
#define CNSDK_PATCH_VERSION 143

BEGIN_CAPI_DECL

LEIA_COMMON_API
const char* leiaCommonGetGitRefspec();

LEIA_COMMON_API
const char* leiaCommonGetGitSha1();

LEIA_COMMON_API
const char* leiaCommonGetVersion();

END_CAPI_DECL
