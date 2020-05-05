#ifdef _WIN32
#pragma comment ( lib, "Shlwapi.lib" )
#endif

#include <benchmark/benchmark.h>
#include "Component.h"
#include "Director.h"

using namespace coco;
using namespace benchmark;

struct Position : public Component<Position>
{
    Position(float X = 0.0, float Y = 0.0) : x(X), y(Y){}
    float x;
    float y;
};

struct Velocity : public Component<Velocity>
{
    Velocity(float X = 0.0, float Y = 0.0) : x(X), y(Y) {}
    float x;
    float y;
};

struct Mass : public Component<Mass>
{
    Mass(float Value = 0.0) : value(Value){}
    float value;
};

class SysPhysics : public System
{
private:
    
public:
    SysPhysics(Director& d):System(d) {
        requires<Position, Velocity, Mass>();
    }
    ~SysPhysics(){}

    void update(float dt) override {
        for(auto e : _entities) {
            auto [pos, v, mass] = _director.getComponents<Position, Velocity, Mass>(e);
            pos.x += v.x;
            pos.y += v.y;
        }
    }
};

/****************
 * Benchmarks
 ****************/
static void CC_Create(State& state) {
    Director d;
    d.addSystem<SysPhysics>(d);

    for (auto _ : state) {
        Entity e = d.createEntity();
        d.addComponent<Position>(e);
        d.addComponent<Velocity>(e);
        d.addComponent<Mass>(e);
    }

    auto nbItems = static_cast<int>(state.iterations()) * state.range();
    state.SetItemsProcessed(static_cast<std::size_t>(nbItems));
    state.SetComplexityN(state.range());
}

BENCHMARK(CC_Create)->RangeMultiplier(10)->Range(100000, 10000000);

static void CC_CreateRemove(State& state) {
    Director d;
    d.addSystem<SysPhysics>(d);

    for (auto _ : state) {
        Entity e = d.createEntity();
        d.addComponent<Position>(e);
        d.addComponent<Velocity>(e);
        d.addComponent<Mass>(e);
    }

    std::vector<Entity> entities = d.getSystems()[0]->getEntities();
    for (auto e : entities) {
        d.removeEntity(e);
    }

    auto nbItems = static_cast<int>(state.iterations()) * state.range();
    state.SetItemsProcessed(static_cast<std::size_t>(nbItems));
    state.SetComplexityN(state.range());
}

BENCHMARK(CC_CreateRemove)->RangeMultiplier(10)->Range(100000, 10000000);

template<int UPDATE_COUNT>
static void CC_Update(State& state) {
    Director d;
    d.addSystem<SysPhysics>(d);

    for (auto _ : state) {
        Entity e = d.createEntity();
        d.addComponent<Position>(e);
        d.addComponent<Velocity>(e);
        d.addComponent<Mass>(e);
    }

    for (int i = 0; i < UPDATE_COUNT; i++)
    {
        d.update(0.01f);
    }
    
    std::vector<Entity> entities = d.getSystems()[0]->getEntities();
    for (auto e : entities) {
        d.removeEntity(e);
    }

    auto nbItems = static_cast<int>(state.iterations()) * state.range();
    state.SetItemsProcessed(static_cast<std::size_t>(nbItems));
    state.SetComplexityN(state.range());
}

BENCHMARK_TEMPLATE(CC_Update, 10)->RangeMultiplier(10)->Range(100000, 10000000);

BENCHMARK_MAIN();
