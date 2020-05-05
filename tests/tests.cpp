#include "gtest/gtest.h"

#include "Director.h"
#include "System.h"
#include "ComTransform.h"
#include "ComPhysics.h"

using namespace coco;
using namespace std;

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

// Setup test
class CoreTest : public ::testing::Test
{
protected:
    Director manager;

    CoreTest() {
        manager.addSystem<SysPhysics>(manager);
    }

    ~CoreTest() override {
    }
};

TEST_F(CoreTest, Entity) {
    auto e1 = manager.createEntity();
    auto e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 1);

    auto e2 = manager.createEntity();
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 2);

    auto e3 = manager.createEntity();
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 3);

    //remove entity
    manager.removeEntity(e1);
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 2);

    //remove invalid entity
    manager.removeEntity(e1);
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 2);

    manager.removeEntity(e2);
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 1);

    manager.removeEntity(e3);
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 0);

    manager.removeEntity(e3);
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 0);

    //Get entity from the pool
    e1 = manager.createEntity();
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 1);

    e2 = manager.createEntity();
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 2);

    e3 = manager.createEntity();
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 3);

    EXPECT_NE(e2, e3);

    manager.removeEntity(e1);
    manager.removeEntity(e2);
    manager.removeEntity(e3);
}

TEST_F(CoreTest, RemoveEntity) {
    for (int i = 0; i < 100; i++)
    {
        auto e = manager.createEntity();
        manager.addComponent<ComTransform>(e);
        manager.addComponent<ComPhysics>(e, 1.0f, 1.0f, 1.0f);
    }
    auto s_count = manager.getEntityCount();
    EXPECT_EQ(s_count, 100);

    auto systems = manager.getSystems();
    auto s = systems[0];
    vector<Entity> entities = s->getEntities();
    for(auto e : entities){
        manager.removeEntity(e);
    }
    s_count = manager.getEntityCount();
    EXPECT_EQ(s_count, 0);
}

TEST_F(CoreTest, Component) {
    auto e1 = manager.createEntity();
    manager.addComponent<ComTransform>(e1);
    manager.addComponent<ComPhysics>(e1, 1.0f, 2.0f, 3.0f);

    auto e2 = manager.createEntity();
    manager.addComponent<ComTransform>(e2);

    //System count
    //only 1 meets the requirements
    auto systems = manager.getSystems();
    auto s = systems[0];
    auto s_count = s->count();
    EXPECT_EQ(s_count, 1);

    //System count
    //Remove all compoents
    manager.removeComponent<ComPhysics>(e1);
    manager.removeComponent<ComPhysics>(e1);
    manager.removeComponent<ComPhysics>(e2);
    s_count = s->count();
    EXPECT_EQ(s_count, 0);

    //System count
    //Add components back again
    manager.addComponent<ComPhysics>(e1, 1.0f, 2.0f, 3.0f);
    manager.addComponent<ComPhysics>(e2, 2.0f, 3.0f, 4.0f);
    s_count = s->count();
    EXPECT_EQ(s_count, 2);

    //Component count
    //Add duplicated components
    manager.addComponent<ComTransform>(e1);
    manager.addComponent<ComTransform>(e1);
    auto c_count = manager.getComponentCount<ComTransform>();
    EXPECT_EQ(c_count, 2);

    //Remove entity
    auto e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 2);
    manager.removeEntity(e1);
    manager.removeEntity(e2);
    s_count = s->count();
    EXPECT_EQ(s_count, 0);
    c_count = manager.getComponentCount<ComTransform>();
    EXPECT_EQ(c_count, 0);
    e_count = manager.getEntityCount();
    EXPECT_EQ(e_count, 0);
}

TEST_F(CoreTest, System) {
    auto e1 = manager.createEntity();
    manager.addComponent<ComTransform>(e1);
    manager.addComponent<ComPhysics>(e1, 1.0f, 2.0f, 3.0f);

    auto e2 = manager.createEntity();
    manager.addComponent<ComTransform>(e2);
    manager.addComponent<ComPhysics>(e2, 2.0f, 2.0f, 2.0f);

    manager.update(1.0f);

    auto ct1 = manager.getComponent<ComTransform>(e1);
    auto cp1 = manager.getComponent<ComPhysics>(e1);

    auto pos = ct1.getPos();
    EXPECT_FLOAT_EQ(pos.x, 1.0f);
    EXPECT_FLOAT_EQ(pos.y, 2.0f);
    EXPECT_FLOAT_EQ(pos.z, 3.0f);

    auto [ct2, cp2] = manager.getComponents<ComTransform, ComPhysics>(e2);
    pos = ct2.getPos();
    EXPECT_FLOAT_EQ(pos.x, 2.0f);
    EXPECT_FLOAT_EQ(pos.y, 2.0f);
    EXPECT_FLOAT_EQ(pos.z, 2.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
