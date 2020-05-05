#pragma once

#include <vector>
#include <bitset>
#include <set>
#include "Common.h"

#define ComponentCount 32

using ComponentMask = std::bitset<ComponentCount>;
using EntityList = std::vector<std::bitset<ComponentCount>>;

namespace coco
{
    using Entity = uint32_t;
    using Index = uint32_t;
    static constexpr auto InvalidIndex = std::numeric_limits<Index>::max();

    class EntityContainer
    {
    private:
        //Store released entities
        std::set<Entity> _entityPool;
        //Record the type of component that an entity has
        //Entity Component Mapping
        EntityList _ecMapping;
        //Active entities
        std::vector<Entity> _entities;
    public:
        EntityContainer() {}
        ~EntityContainer() {}

        Entity create() {
            Entity entity;
            if(_entityPool.empty()){
                //Create new entity
                entity = static_cast<Index>(_ecMapping.size());
                _ecMapping.emplace_back();
            } else {
                entity = *_entityPool.begin();
                _entityPool.erase(entity);
                _ecMapping[entity].reset();
            }
            return entity;
        }

        void remove(Entity e){
            _entityPool.insert(e);
        }

        Entity count() const {
            return static_cast<Entity>(_ecMapping.size() - _entityPool.size());
        }

        EntityList& getMapping() {
            return _ecMapping;
        }

        void setComponentMask(Entity e, ComType type, bool value) {
            _ecMapping[e][type] = value;
        }
        
        const ComponentMask& getComponentMask(Entity e) const {
            return _ecMapping[e];
        }

    };
} // namespace coco
