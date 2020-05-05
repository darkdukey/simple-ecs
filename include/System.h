#pragma once


namespace coco
{
    class Director;
    class System
    {
    public:
        System(Director& d):_director(d) {}
        virtual ~System() = default;
        //Update function
        virtual void update(float dt) = 0;
        auto count() {
            return _entities.size();
        }
        auto& getEntities() {
            return _entities;
        }
    protected:
        Director& _director;
        std::vector<Entity> _entities;
    protected:
        //Setup component requirements for system
        template<typename ...Ts>
        void requires() {
            (_requirements.set(Ts::type), ...);
        }
    private:
        friend Director;
        std::bitset<ComponentCount> _requirements;
        std::unordered_map<Entity, Index> _mapping;

    private:
        void onEntityUpdate(Entity e, const std::bitset<ComponentCount>& components) {
            bool fit = (_requirements & components) == _requirements;
            bool owned = _mapping.find(e) != _mapping.end();
            if(!owned && fit){
                add(e);
            } else if(owned && !fit){
                remove(e);
            }
        }
        void onEntityRemoved(Entity e){
            if(_mapping.find(e) != _mapping.end()){
                remove(e);
            }
        }
        void add(Entity e) {
            _mapping[e] = static_cast<Index>(_entities.size());
            _entities.emplace_back(e);
        }
        void remove(Entity e) {
            //Find entity index
            auto index = _mapping[e];
            //Replace the index with back of entity list
            _mapping[_entities.back()] = index;
            _mapping.erase(e);
            _entities[index] = _entities.back();
            _entities.pop_back();
        }
    };
} // namespace coco
