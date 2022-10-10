//
//  scene.h
//  solong
//
//  Created by Dmitry Shipicyn on 05.07.2022.
//

#ifndef scene_h
#define scene_h

#include "entity.h"
#include "physics.h"

#include <vector>

class Game;

class Scene
{
public:
    using ptr = std::unique_ptr<Scene>;
    
    Scene();
    virtual ~Scene() noexcept;
    
    virtual void start(Game& game);
    virtual void update(Game& game);
    virtual void render(Game& game);
    
    void attach_entity(Entity::ptr ptr);
    void detach_entity(Entity::ptr ptr);

	void set_physic_world(physics::World::ptr world);
	physics::World* get_physic_world();
    
private:
	using AttachedEntity = std::pair<bool, Entity::ptr>;
    std::vector<AttachedEntity> entities_;
    std::vector<AttachedEntity> added_entities_;
	physics::World::ptr			world_;
};

#endif /* scene_h */
