#pragma once

#include "leia/headTracking/engine/api.h"
#include "leia/headTracking/common/types.hpp"

#include <glm/glm.hpp>

namespace leia {
namespace head {

struct KalmanCoeffs {
    glm::vec3 a = glm::vec3(0.02f, 0.05f, 0.2f);
    glm::vec3 b = glm::vec3(0.1f);
};

class KalmanFilter {
public:
    LHT_ENGINE_API
    void Invalidate() { _isValid = false; }

    /// Reset to \param pos. KalmanFilter becomes valid.
    LHT_ENGINE_API
    void Reset(glm::vec3 const& pos);

    /// Update taking into account current \param measuredPos and elapsed time \param dt.
    /// KalmanFilter must be valid.
    LHT_ENGINE_API
    void Update(KalmanCoeffs const& coeffs, glm::vec3 const& measuredPos, float dt);

    /// Predict position of \param movingPoint after \param dt time passes.
    LHT_ENGINE_API
    static glm::vec3 Predict(MovingPoint const& movingPoint, float dt);

    LHT_ENGINE_API
    glm::vec3 Predict(float dt);

    bool isValid() const { return _isValid; }

    MovingPoint const& point() const { return _point; }

private:
    bool _isValid = false;
    MovingPoint _point;
};

} // namespace head
} // namespace leia
