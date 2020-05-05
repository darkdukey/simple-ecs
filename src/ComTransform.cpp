#include "ComTransform.h"
#include "glm/ext.hpp"

using namespace glm;

namespace coco {
    void ComTransform::setPos(float x, float y, float z) {
        _transform = translate(_transform, {x, y, z});
    }
    void ComTransform::setPos(const vec3& pos) {
        _transform = translate(_transform, pos);
    }

    vec3 ComTransform::getPos() {
        return vec3(_transform[3]);
    }
} // namespace coco