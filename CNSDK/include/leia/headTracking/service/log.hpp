#pragma once

#include "leia/common/log.hpp"
#include "leia/headTracking/service/common.hpp"

std::string SupportedFaceDetectorBackendsToString(uint32_t v);
DEFINE_CUSTOM_FORMATTING(leia::head::ServerInfo,
     "{{.version={}, .supportedFaceDetectorBackends={}}}",
     v.version, SupportedFaceDetectorBackendsToString(v.supportedFaceDetectorBackends));
DEFINE_CUSTOM_FORMATTING(leia::head::ServiceError,
     "{}, {}", ToStr(v.code), v.message);
DEFINE_CUSTOM_FORMATTING(leia::head::ClientServerInitArgs,
     ".logLevel={}, .faceDetectorConfig={}, .startTracking={}, .enableProfiling={}",
     leia_log_level_to_string(v.logLevel), v.faceDetectorConfig, v.startTracking, v.enableProfiling);
