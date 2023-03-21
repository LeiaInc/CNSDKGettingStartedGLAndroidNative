#pragma once

#include "leia/common/types.h"
#include "leia/common/types.hpp"

#include <glm/glm.hpp>

namespace leia {

static_assert(kLeiaTimestampSpaceSystem == int(Timestamp::Space::System));
static_assert(kLeiaTimestampSpaceUnknown == int(Timestamp::Space::Unknown));

inline leia_vector3 FromCxx(glm::vec3 const& in) {
    return {in.x, in.y, in.z};
}

inline leia_vector2d FromCxx(glm::dvec2 const& in) {
    return {in.x, in.y};
}

inline leia_timestamp FromCxx(Timestamp const& timestamp) {
    return {
        static_cast<leia_timestamp_space>(timestamp.space),
        timestamp.ms,
    };
}

inline glm::dvec2 ToCxx(leia_vector2d const& v) {
    return {v.x, v.y};
}

inline glm::vec3 ToCxx(leia_vector3 const& v) {
    return {v.x, v.y, v.z};
}

} // namespace leia
