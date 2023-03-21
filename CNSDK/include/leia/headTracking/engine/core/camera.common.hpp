#pragma once

#include "leia/headTracking/engine/core/camera.hpp"

namespace leia::head {

class TrivialCameraFrame : public CameraFrame {
public:
    TrivialCameraFrame(ImageDesc const& imageDesc, Timestamp timestamp)
        : _imageDesc(imageDesc), _timestamp(timestamp) {}
    ~TrivialCameraFrame() override = default;

    Timestamp timestamp() const override { return _timestamp; }
    Slice<const ImageDesc> imageDescs() const override { return {&_imageDesc, 1}; }
    bool GetDepth(glm::dvec2, float*) const override { return false; }
    bool DeprojectPixelToPoint(glm::dvec2, float, glm::vec3*) const override { return false; }
    bool ProjectPointToPixel(glm::vec3 const&, glm::dvec2*, float*) const override { return false; }

private:
    ImageDesc _imageDesc;
    Timestamp _timestamp;
};

} // namespace leia::head
