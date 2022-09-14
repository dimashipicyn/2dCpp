//
//  scene.h
//  solong
//
//  Created by Dmitry Shipicyn on 05.07.2022.
//

#ifndef scene_h
#define scene_h

#include "entity.h"

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
    
    void update_internal(Game& game);
    
private:
	using AttachedEntity = std::pair<bool, Entity::ptr>;
    std::vector<AttachedEntity> entities_;
    std::vector<AttachedEntity> added_entities_;
};

#endif /* scene_h */
