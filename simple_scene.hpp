//
//  simple_scene.hpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 11.09.2022.
//

#ifndef simple_scene_hpp
#define simple_scene_hpp

#include <memory>

#include "scene.h"
#include "Sprite.h"
#include "AnimationSprite.hpp"

class Simple_scene : public Scene
{
public:
    virtual ~Simple_scene() noexcept;
    virtual void start(Game& game) override;
    virtual void update(Game& game) override;
	virtual void render(Game& game) override;
    
private:
	Sprite sprite_;
	AnimationSprite anim_sprite_;
	Entity::ptr ball_;
	Entity::ptr rocket_;
	
};

#endif /* simple_scene_hpp */
