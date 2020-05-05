#pragma once

#include <array>
#include <memory>
#include <tuple>
#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace coco {
    class Director
    {
    private:
        std::vector<std::unique_ptr<BaseComContainer>> _containers;
        EntityContainer _entityContainer;
        std::vector<std::shared_ptr<System>> _systems;

    public:
        Director() {
            ComType count = BaseComponent::size();
            for (ComType i = 0; i < count; i++)
            {
                _containers.emplace_back(BaseComponent::create(i));
            }
            
        }
        ~Director() {}

        //System
        template<typename T, typename ...Args>
        void addSystem(Args&& ...args) {
            auto& system = _systems.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
        }

        auto& getSystems() {
            return _systems;
        }

        void update(float dt) {
            for(const auto& s : _systems){
                s->update(dt);
            }
        }

        // Components
        template<typename T, typename... Args>
        void addComponent(Entity e, Args&&... args) {
            getContainer<T>()->add(e, std::forward<Args>(args)...);
            _entityContainer.setComponentMask(e, T::type, true);
            for (auto& s : _systems) {
                auto mask = _entityContainer.getComponentMask(e);
                s->onEntityUpdate(e, mask);
            }
        }

        template<typename T>
        void removeComponent(Entity e){
            getContainer<T>()->remove(e);
            _entityContainer.setComponentMask(e, T::type, false);
            for (auto& s : _systems) {
                //component container will update the mask
                auto mask = _entityContainer.getComponentMask(e);
                s->onEntityUpdate(e, mask);
            }
        }

        template<typename... Ts>
        std::tuple<Ts&...> getComponents(Entity e){
            return std::tie(getComponent<Ts>(e)...);
        }

        template<typename T>
        T& getComponent(Entity e){
            return getContainer<T>()->get(e);
        }

        //Entity
        Entity createEntity(){
            return _entityContainer.create();
        }
        void removeEntity(Entity e){
            // Remove components
            for (auto& c : _containers){
                c->remove(e);
            }

            // Send message to systems
            for (auto& system : _systems){
                system->onEntityRemoved(e);
            }
            _entityContainer.remove(e);
        }

        //Utility
        size_t getEntityCount(){
            return _entityContainer.count();
        }

        template<typename T>
        size_t getComponentCount(){
            return getContainer<T>()->size();
        }
    private:
    
        template<typename T>
        auto getContainer(){
            return static_cast<ComContainer<T>*>(_containers[T::type].get());
        }
    };
} // namespace coco
