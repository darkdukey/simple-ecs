#include "Node.h"
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream> 
#include <sstream>  

using namespace glm;
using namespace std;

namespace coco {
    void Node::setPos(float x, float y, float z) {
        _transform = translate(_transform, {x, y, z});
    }
    void Node::setPos(const vec3& pos) {
        _transform = translate(_transform, pos);
    }

    vec3 Node::getPos() {
        return vec3(_transform[3]); 
    }

    string Node::str() {
        stringstream b;
        vec3 p = getPos();
        b << "== Node ==" << endl;
        b << "== pos\t x:" << p.x << " y:" << p.y << " z:" << p.z << endl;
        b << "== matrix " << endl;
        b << glm::to_string(_transform) << endl;
        return b.str();
    }
    
}
