#pragma once

#include <glm/glm.hpp>
#include <string>
#include <cstdint>

namespace coco {
    class Node
    {
    public:
        Node() {
            _transform = glm::mat4(1.0f);
        }
        ~Node() {}
        void setPos(float x, float y, float z);
        void setPos(const glm::vec3& pos);
        glm::vec3 getPos();
        std::string str();
    private:
        uint32_t _id;
        glm::mat4 _transform;
    };
}
