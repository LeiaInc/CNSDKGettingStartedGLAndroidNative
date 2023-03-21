#pragma once

#include "leia/common/log.h"
#include "leia/common/types.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <spdlog/spdlog.h>

#define DEFINE_CUSTOM_FORMATTING(Type, ...) \
    template <> struct fmt::formatter<Type> { \
        constexpr auto parse(format_parse_context& ctx) { \
            return ctx.begin(); \
        } \
        template <typename FormatContext> \
        auto format(Type const& v, FormatContext& ctx) -> decltype(ctx.out()) { \
            return fmt::format_to(ctx.out(), __VA_ARGS__); \
        } \
    }

template <typename T> struct fmt::formatter<glm::vec<2, T, glm::defaultp>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(glm::vec<2, T, glm::defaultp> const& p, FormatContext& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "{{{:g}, {:g}}}", p.x, p.y);
    }
};
template <typename T> struct fmt::formatter<glm::vec<3, T, glm::defaultp>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(glm::vec<3, T, glm::defaultp> const& p, FormatContext& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "{{{:g}, {:g}, {:g}}}", p.x, p.y, p.z);
    }
};
template <> struct fmt::formatter<glm::mat3> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(const glm::mat3& v, FormatContext& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "{{{}, {}, {}}}", v[0], v[1], v[2]);
    }
};

inline const char* ToString(leia::Timestamp::Space space) {
    switch (space) {
        case leia::Timestamp::Space::System:
            return "System";
        default:
            return "Unknown";
    }
}
DEFINE_CUSTOM_FORMATTING(leia::Timestamp::Space,
     "Timestamp::Space::{}", ToString(v));
DEFINE_CUSTOM_FORMATTING(leia::Timestamp,
     "{{.space={}, .ms={}}}", v.space, v.ms);
DEFINE_CUSTOM_FORMATTING(leia::ImageDesc,
     "{{.width={}, .height={}, .rotation={}}}", v.width, v.height, v.rotation);
DEFINE_CUSTOM_FORMATTING(leia::CameraIntrinsics,
     "{{.width={}, .height={}, .ppx={}, .ppy={}, .fx={}, .fy={}, .isMirrored={}}}",
     v.width, v.height, v.ppx, v.ppy, v.fx, v.fy, v.isMirrored);
DEFINE_CUSTOM_FORMATTING(leia::FaceDetectorConfig,
     "{{.backend={}, .inputType={}}}",
     leia::ToStr(v.backend), leia::ToStr(v.inputType));
DEFINE_CUSTOM_FORMATTING(leia::SourceLocation,
     "{} {}:{}", v.funcname, v.filename, v.line);

namespace leia {

LEIA_COMMON_API
std::shared_ptr<spdlog::logger> GetDefaultLogger();

LEIA_COMMON_API
spdlog::level::level_enum ToSpdlog(leia_log_level);

LEIA_COMMON_API
leia_log_level FromSpdlog(spdlog::level::level_enum);

} // namespace leia
