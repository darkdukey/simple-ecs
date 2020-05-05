#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <chrono>
#include <iostream>

#include "Director.h"
#include "ComTransform.h"
#include "ComPhysics.h"
#include "System.h"

using namespace std;
using namespace glm;
using namespace coco;

class SysPhysics : public System
{
private:
    
public:
    SysPhysics(Director& d):System(d) {
        requires<ComTransform, ComPhysics>();
    }
    ~SysPhysics(){}

    void update(float dt) override {
        for(auto e : _entities) {
            auto [trans, phy] = _director.getComponents<ComTransform, ComPhysics>(e);
            auto p = trans.getPos();
            p += phy.getVelocity();
            trans.setPos(p);
        }
    }
};

int main(int argc, char **argv)
{
    constexpr size_t nbEntities = 10000;
    constexpr size_t nbUpdates = 100;
    Director manager;
    auto sys_physics = SysPhysics(manager);
    // manager.addSystem(sys_physics);
    // manager.reserve(nbEntities);

    for (auto i = size_t(0); i < nbEntities; ++i)
    {
        auto entity = manager.createEntity();
        manager.addComponent<ComTransform>(entity);
        manager.addComponent<ComPhysics>(entity, 1.0f, 2.0f, 3.0f);
    }

    //Simulate update loop
    auto prevTime = chrono::system_clock::now();
    for (auto i = size_t(0); i < nbUpdates; ++i)
    {
        auto time = chrono::system_clock::now();
        auto dt = chrono::duration<float>(time - prevTime).count();
        prevTime = time;
        // manager.update(dt);
    }
    return 0;
}