//
//  simple_scene.cpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 11.09.2022.
//

#include "simple_scene.hpp"
#include "game.h"
#include "graphics.h"
#include "Sprite_atlas.hpp"
#include "log.h"

#include <cmath>
#include <SDL.h>
#include <fstream>

class SimpleBall : public Entity
{
public:
	SimpleBall(const Sprite& sprite)
		: ball_(sprite)
	{

	};
	virtual ~SimpleBall() {};

	virtual void input(Game& game) override {};

	virtual void update(Game& game) override {
		if (get_velosity() > 0.7) {
			set_velosity(0.7);
		}
		translate(get_velosity() * game.get_tick());
	}

	virtual void render(Game& game) override {
		auto pos = get_position();
		auto dir = get_direction();
		pos = pos.add(dir.scalar(game.get_lag() * get_velosity()));
		int x = pos.x - ball_.get_width() / 2;
		int y = pos.y - ball_.get_height() / 2;
		ball_.set_position(x, y);
		ball_.draw(game.get_graphics());
	}

	Sprite ball_;
};

class Rocket : public Entity
{
public:
	Rocket() {

	};
	virtual ~Rocket() {};

	virtual void input(Game& game) override {};

	virtual void update(Game& game) override {
		auto x_input = game.get_input().get_horizontal_axis();
		if (x_input) {
			set_direction(Vec2f(x_input, 0).normalize());
			set_velosity(0.5);
			translate(get_velosity() * game.get_tick());
		}
		else {
			set_direction(Vec2f(0, 0));
			set_velosity(0);
		}
	}

	virtual void render(Game& game) override {
		auto pos = get_position();
		auto dir = get_direction();
		pos = pos.add(dir.scalar(game.get_lag() * get_velosity()));
		square.x = pos.x - square.w / 2;
		square.y = pos.y - square.h / 2;
		game.get_graphics().draw_square(square, {0xFF,0xFF,0,0xFF});
	}

	Rect square;
};

class Block : public Entity
{
public:
	Block(const Sprite& sprite)
		: block_(sprite)
	{
	};
	virtual ~Block() {};

	virtual void input(Game& game) override {};

	virtual void update(Game& game) override {
	}

	virtual void render(Game& game) override {
		//game.get_graphics().draw_square(square, {0xFF,0xFF,0,0xFF});
		block_.draw(game.get_graphics());
	}

	Sprite block_;
};

struct Level {
	std::vector<int> blocks;
	int size_x;
	int size_y;
	bool load_in_file(const std::string& filename) {
		std::ifstream file(filename);
		if (file.is_open()) {
			blocks.clear();
			file >> size_x;
			file >> size_y;
			while (!file.eof()) {
				int num = -1;
				file >> num;
				if (num != -1) {
					blocks.push_back(num);
				}
			}
			if (blocks.size() == size_x * size_y) {
				return true;
			}
		}
		return false;
	}
};

void Simple_scene::update(Game &game) {
	//Scene::update(game);

	if (ball_->get_velosity() < 0.1) {
		float button = game.get_input().get_button(Input::Button::MouseLeft);
		if (button > 0.1) {
			ball_->set_velosity(0.5);
			ball_->set_direction(Vec2f(0, -1));
		}
	}

	auto w = get_physic_world();
	blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&w](Entity::ptr& block) {
		if (block->get_body()->has_collision()) {
			w->remove_body(block->get_body());
			return true;
		}
		return false;
	}), blocks.end());

	ball_->update(game);
	rocket_->update(game);
	get_physic_world()->step(0);
}


void Simple_scene::start(Game &game) {
	SET_LOG_LEVEL(LOG_LEVEL_INFO);

	auto width = game.get_graphics().get_width();
	auto height = game.get_graphics().get_height();

	auto w = std::make_unique<physics::World>(1.5);

	Sprite ball_sprite;
	ball_sprite.load(game.get_graphics(), "assets/ball.png");
	auto ball = std::make_shared<SimpleBall>(ball_sprite);
	auto rocket = std::make_shared<Rocket>();

	auto ball_body = std::make_shared<physics::Body>(physics::AABB(width / 2.0 - 10, height - 50, 20, 20));
	auto rocket_body = std::make_shared<physics::Body>(physics::AABB(width / 2.0 - 50, height - 30, 100, 20));

	ball_body->set_active(true);
	ball_body->set_layer(physics::Layer::LAYER_1);
	ball_body->set_owner(ball);
	ball->set_body(ball_body);
	//ball->square = Rect{width / 2 - 20, height - 50, 20, 20};
	w->add_body(ball_body, true);

	rocket_body->set_active(true);
	rocket_body->set_layer(physics::Layer::LAYER_1);
	rocket_body->set_owner(rocket);
	rocket->set_body(rocket_body);
	rocket->square = Rect{width / 2 - 50, height - 30, 100, 20};
	w->add_body(rocket_body, true);

	ball_ = std::move(ball);
	rocket_ = std::move(rocket);


	// create walls
	physics::AABB walls_aabb[3] = {
		{-20, -20, 20 ,static_cast<float>(height + 20)},
		{static_cast<float>(width), -20, 20, static_cast<float>(height + 20)},
		{0,-20,static_cast<float>(width), 20}
	};

	for (int i = 0; i < 3; i++) {
		auto wall_aabb = walls_aabb[i];
		auto wall_body = std::make_shared<physics::Body>(wall_aabb);
		wall_body->set_active(true);
		wall_body->set_layer(physics::Layer::LAYER_1);

		w->add_body(wall_body, false);
		walls[i] = std::move(wall_body);
	}

	Texture block_simple;
	Texture block_solid;
	block_simple.load(game.get_graphics(), "assets/block.png");
	block_solid.load(game.get_graphics(), "assets/block_solid.png");
	Sprite solid(block_solid);
	Sprite simple(block_simple);

	Level level;
	level.load_in_file("levels/1.txt");

	//create blocks
	int x = 0;
	int y = 0;
	int size_x = width / level.size_x;
	int size_y = height / 2 / level.size_y;
	simple.set_size(size_x, size_y);
	solid.set_size(size_x, size_y);

	Color colors[] = {
		{255,0,0},
		{255,255,0},
		{255,0,255},
		{0,255,0},
		{0,255,255},
		{0,0,255}
	};
	for (const auto& b : level.blocks) {
		auto s = (b == 1 ? solid : simple);
		s.set_position(x, y);
		s.set_color(colors[b]);
		auto block = std::make_shared<Block>(s);
		physics::AABB block_aabb(x, y, size_x, size_y);
		auto block_body = std::make_shared<physics::Body>(block_aabb);
		block_body->set_active(true);
		block_body->set_layer(physics::Layer::LAYER_1);
		block_body->set_owner(block);
		block->set_body(block_body);
		w->add_body(block->get_body(), false);
		blocks.emplace_back(std::move(block));
		x += size_x;

		if (x == width) {
			y += size_y;
		}
		x %= width;
	}

	set_physic_world(std::move(w));
}

Simple_scene::~Simple_scene() noexcept { 
}

void Simple_scene::render(Game &game) { 
//	sprite_.draw(game.get_graphics());
//	anim_sprite_.draw(game.get_graphics(), game.get_tick());
	ball_->render(game);
	rocket_->render(game);

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->render(game);
	}
	//game.get_graphics().draw_square({220,220,10,10}, {0x0,0xFF,0,0xFF});
}

