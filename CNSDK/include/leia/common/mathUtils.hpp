#pragma once

#include "leia/common/api.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace leia {

LEIA_COMMON_API
bool epsilonEqual(glm::mat3 const& m1, glm::mat3 const& m2, float eps);

LEIA_COMMON_API
glm::mat3 GetRotationMatFromRodriguesNotation(glm::vec3 const& rotation);

} // namespace leia
