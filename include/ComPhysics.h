#pragma once

#include <glm/glm.hpp>
#include "Component.h"

namespace coco
{
    class ComPhysics : public Component<ComPhysics>
    {
    private:
        glm::vec3 velocity;
    public:
        ComPhysics(float x, float y, float z) {
            velocity.x = x;
            velocity.y = y;
            velocity.z = z;
        }
        glm::vec3 getVelocity() {
            return velocity;
        }
    };   
} // namespace coco