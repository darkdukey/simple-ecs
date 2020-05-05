#pragma once

#include <glm/glm.hpp>
#include "Component.h"

namespace coco{
    class ComTransform : public Component<ComTransform>
    {
    private:
        glm::mat4 _transform;
    public:
        ComTransform(){
            _transform = glm::mat4( 1.0 );
        }
        void setPos(float x, float y, float z);
        void setPos(const glm::vec3& pos);
        glm::vec3 getPos();
    };
} // namespace coco