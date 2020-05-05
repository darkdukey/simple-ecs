#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "Common.h"
#include "Entity.h"

namespace coco {

#pragma region Container
    //Component container honds components for continues memory access
    class BaseComContainer 
    {
    public:
        virtual ~BaseComContainer() = default;
        virtual void remove(Entity e) = 0;
        virtual size_t size() = 0;
    };

    template<typename T>
    class ComContainer : public BaseComContainer
    {
    protected:
        //Component list
        std::vector<T> _components;
        //Entity list that have this component
        std::vector<Entity> _entities;
        //Maping component index with entity
        std::unordered_map<Entity, Index> _entityMapping;

    public:
        virtual ~ComContainer() = default;

        template<typename... Args>
        void add(Entity e, Args&&... args) {
            if(_entityMapping.find(e) == _entityMapping.end()){
                auto index = static_cast<Index>(_components.size());
                //Create new component
                _components.emplace_back(std::forward<Args>(args)...);
                _entities.emplace_back(e);
                //Create mapping
                _entityMapping[e] = index;
            }
        }
        
        void remove(Entity e) override{
            //Find entity index
            if(_entityMapping.find(e) != _entityMapping.end()) {
                if(_components.size() <= 1){
                    _components.clear();
                    _entities.clear();
                    _entityMapping.clear();
                } else {
                    auto index = _entityMapping[e];
                    //Remoev from component list
                    _components[index] = std::move(_components.back());
                    _components.pop_back();
                    //Remove from entity list
                    _entities[index] = _entities.back();
                    _entities.pop_back();
                    //Remove from entity mapping
                    _entityMapping[_entities.back()] = index;
                    _entityMapping.erase(e);
                }
            }
        }

        auto& get(Entity e){
            auto index = _entityMapping[e];
            return _components[index];
        }

        size_t size() override {
            return _components.size();
        }
    };
#pragma endregion

#pragma region Component
    //Component
    class BaseComponent
    {
    private:
        using ContainerCtor = std::unique_ptr<BaseComContainer>(*)();
        static std::vector<ContainerCtor>& getFactory() {
            static std::vector<ContainerCtor> sFactories;
            return sFactories;
        }
    protected:
        template<typename T>
        static ComType getType() {
            getFactory().push_back([]() -> std::unique_ptr<BaseComContainer> {
                return std::make_unique<ComContainer<T>>();
            });
            return static_cast<ComType>(getFactory().size() - 1);
        }
    public:
        static ComType size() {
            return static_cast<ComType>(getFactory().size());
        }
        static std::unique_ptr<BaseComContainer> create(ComType type){
            if(type >= size()){
                // TODO: log error message
                type = 0;
            }
            return getFactory()[type]();
        }
    };

    template<typename T>
    class Component : public BaseComponent
    {
    public:
        // static const ComType type = BaseComponent::getType<T>();
        static const ComType type;
    };

    template<typename T>
    const ComType Component<T>::type = BaseComponent::getType<T>();
#pragma endregion
}

