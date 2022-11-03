//
//  simple_scene.hpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 11.09.2022.
//

#ifndef simple_scene_hpp
#define simple_scene_hpp

#include <memory>
#include <vector>

#include "scene.h"
#include "Sprite.h"
#include "AnimationSprite.hpp"
#include "physics.h"

class Ball;
class Rocket;
class Block;

class Simple_scene : public Scene
{
public:
    virtual ~Simple_scene() noexcept;
    virtual void start(Game& game) override;
    virtual void update(Game& game) override;
	virtual void render(Game& game) override;
    
private:
	void create_walls(int width, int height);
	void load_level(Game& game, int width, int height, int num);
	void create_rocket(Game& game, int width, int height);
	void create_ball(Game& game, int width, int height);
	Vec2f get_ball_start_pos();
	void next_level(Game& game);

	std::shared_ptr<Ball> ball_;
	std::shared_ptr<Rocket> rocket_;
	physics::Body::ptr walls[3];
	std::vector<std::shared_ptr<Block>> blocks;
	int current_level = 1;
};

#endif /* simple_scene_hpp */
