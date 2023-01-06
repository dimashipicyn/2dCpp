//
//  simple_scene.cpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 11.09.2022.
//

#include "stdafx.h"

#include "simple_scene.hpp"
#include "game.h"
#include "graphics.h"
#include "Sprite_atlas.hpp"
#include "log.h"

#include <cmath>
#include <SDL.h>
#include <fstream>
#include <algorithm>

Simple_scene::~Simple_scene() noexcept {
}

class Ball : public Entity
{
public:
	Ball(const Rect& rect, const Color& color)
		: rect_(rect)
		, color_(color)
	{

	};
	virtual ~Ball() {};

	virtual void input(Game& game) override {
		if (!run_) {
			float button = game.get_input().get_button(Input::Button::MouseLeft);
			if (button > 0.1) {
				set_velosity(0.2);
				set_direction(Vec2f(0.5, -0.5));
				run_ = true;
			}
		}
	};

	virtual void update(Game& game) override {
		//if (!run_) {
		//	return;
		//}

		Simple_scene* scene = static_cast<Simple_scene*>(game.get_active_scene());
		auto ball_pos = get_position();
		int height = game.get_graphics().get_height();
		if (ball_pos.y > height) {
			run_ = false;
			set_position(scene->get_ball_start_pos());
			set_velosity(0);
		}

		if (!run_) {
			float new_velocity = get_velosity();
			set_velosity(new_velocity + 0.0001);
		}

		translate(get_velosity() * game.get_tick());
	}

	virtual void render(Game& game) override {
		auto pos = get_position();
		auto dir = get_direction();
		pos = pos.add(dir.scalar(game.get_lag() * get_velosity()));
		int x = pos.x - rect_.w / 2;
		int y = pos.y - rect_.h / 2;
		game.get_graphics().draw_square({ x, y, rect_.w, rect_.h }, {color_.r, color_.g, color_.b, color_.a});
	}

	Rect rect_;
	Color color_;
	bool run_ = false;
};

class Rocket : public Entity
{
public:
	Rocket(const Rect& rect, const Color& color)
		: rect_(rect)
		, color_(color)
	{

	};
	virtual ~Rocket() {};

	virtual void input(Game& game) override {
		auto x_input = game.get_input().get_horizontal_axis();
		if (x_input) {
			set_direction(Vec2f(x_input, 0).normalize());
			set_velosity(1.5);
		}
		else {
			set_direction(Vec2f(0, 0));
			set_velosity(0);
		}
	};

	virtual void update(Game& game) override {
		translate(get_velosity() * game.get_tick());
		/*auto body = get_body();
		if (body->has_collision()) {
			for (auto& coll : body->get_collisions()) {
				if (auto coll_entity = coll.lock(); coll_entity) {
					if (!coll_entity->get_body()->is_static()) {
						auto save_dir = coll_entity->get_direction();
						coll_entity->set_direction(get_direction());
						coll_entity->translate(get_velosity() * game.get_tick());
						coll_entity->set_direction(save_dir);
					}
				}

			}
		}*/
	}

	virtual void render(Game& game) override {
		auto pos = get_position();
		auto dir = get_direction();
		//if (!get_body()->has_collision()) {
			pos = pos.add(dir.scalar(game.get_lag() * get_velosity()));
		//}
		int x = pos.x - rect_.w / 2;
		int y = pos.y - rect_.h / 2;

		game.get_graphics().draw_square({ x, y, rect_.w, rect_.h }, { color_.r, color_.g, color_.b, color_.a });
	}

	Rect rect_;
	Color color_;
};

class Block : public Entity
{
public:
	Block(const Rect& rect, const Color& color)
		: rect_(rect)
		, color_(color)
	{
	};
	virtual ~Block() {};

	virtual void input(Game& game) override {

	};

	virtual void update(Game& game) override {
		auto body = get_body();
		if (body->has_collision()) {
			game.get_audio().play_chunk("solid");
		}
	}

	virtual void render(Game& game) override {
		game.get_graphics().draw_square({ rect_.x, rect_.y, rect_.w, rect_.h }, { color_.r, color_.g, color_.b, color_.a });
	}

	Rect rect_;
	Color color_;
	bool solid_ = false;
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

void Simple_scene::create_walls(int width, int height) {
	auto w = get_physic_world();

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
		wall_body->set_static(true);

		w->add_body(wall_body, false);
		walls[i] = std::move(wall_body);
	}
}

void Simple_scene::load_level(Game& game, int width, int height, int num) {
	auto w = get_physic_world();

	//Sprite solid;
	//Sprite simple;
	//solid.load(game.get_graphics(), "assets/block_solid.png");
	//simple.load(game.get_graphics(), "assets/block.png");

	Level level;
	level.load_in_file("levels/" + std::to_string(num) + ".txt");

	int x = 0;
	int y = 0;
	int size_x = width / level.size_x;
	int size_y = height / 2 / level.size_y;

	//simple.set_size(size_x, size_y);
	//solid.set_size(size_x, size_y);

	Color colors[] = {
		{255,0,0},
		{255,255,0},
		{255,0,255},
		{0,255,0},
		{0,255,255},
		{0,0,255}
	};

	for (const auto& b : level.blocks) {
		if (b > 0) {
			bool is_solid = (b == 1);

			//Sprite s = (is_solid ? solid : simple);
			//s.set_position(x, y);
			//s.set_color(colors[b]);

			auto block = std::make_shared<Block>(Rect(x, y, size_x, size_y), colors[b]);
			auto block_body = std::make_shared<physics::Body>(physics::AABB(x, y, size_x, size_y));

			block_body->set_active(true);
			block_body->set_layer(physics::Layer::LAYER_1);
			block_body->set_owner(block);
			block_body->set_static(true);
			w->add_body(block_body, false);

			block->set_body(block_body);
			block->solid_ = is_solid;

			blocks.emplace_back(std::move(block));
		}

		x += size_x;
		if (x == width) {
			y += size_y;
		}
		x %= width;
	}
}

void Simple_scene::create_ball(Game& game, int width, int height) {
	auto w = get_physic_world();

	//Sprite ball_sprite;
	//ball_sprite.load(game.get_graphics(), "assets/ball.png");
	//ball_sprite.set_color(Color(0,255,0));

	int min_size = std::min(width, height);
	int rocket_size_h = height / 30;

	int size = min_size / 25;
	int x = width / 2 - size;
	int y = height - rocket_size_h - size / 2 - 1;

	//ball_sprite.set_size(size, size);
	auto ball = std::make_shared<Ball>(Rect(x, y, size, size), Color(0,255,0));
	auto ball_body = std::make_shared<physics::Body>(physics::AABB(x, y, size, size));

	ball_body->set_active(true);
	ball_body->set_layer(physics::Layer::LAYER_1);
	ball_body->set_owner(ball);
	ball_body->set_elasticity(1);
	w->add_body(ball_body, true);

	ball->set_body(ball_body);

	ball_ = std::move(ball);
}

void Simple_scene::create_rocket(Game& game, int width, int height) {
	auto w = get_physic_world();

	//Sprite rocket_sprite;
	//rocket_sprite.load(game.get_graphics(), "assets/paddle.png");

	int size_w = width / 8;
	int size_h = height / 30;
	int x = width / 2 - size_w / 2;
	int y = height - size_h;

	//rocket_sprite.set_position(x, y);
	//rocket_sprite.set_size(size_w, size_h);
	auto rocket = std::make_shared<Rocket>(Rect(x, y, size_w, size_h), Color(255, 0, 0));

	auto rocket_body = std::make_shared<physics::Body>(physics::AABB(x, y, size_w, size_h));

	rocket_body->set_active(true);
	rocket_body->set_layer(physics::Layer::LAYER_1);
	rocket_body->set_owner(rocket);
	w->add_body(rocket_body, true);

	rocket->set_body(rocket_body);

	rocket_ = std::move(rocket);
}

Vec2f Simple_scene::get_ball_start_pos() {
	auto rocket_pos = rocket_->get_position();
	auto rocket_h = rocket_->rect_.h;
	auto ball_h = ball_->rect_.h;
	return Vec2f(rocket_pos.x, rocket_pos.y - rocket_h / 2 - ball_h / 2 - 0.1);
}

void Simple_scene::next_level(Game& game) {
	auto world = get_physic_world();

	std::for_each(blocks.begin(), blocks.end(), [world](auto& block) {
		world->remove_body(block->get_body());
	});

	blocks.clear();

	current_level++;
	int w = game.get_graphics().get_width();
	int h = game.get_graphics().get_height();
	load_level(game, w, h, current_level);

	ball_->set_velosity(0);
	ball_->set_position(get_ball_start_pos());

	auto rocket_pos = rocket_->get_position();
	rocket_->set_position(Vec2f(w / 2, rocket_pos.y));
}

void Simple_scene::start(Game &game) {
	SET_LOG_LEVEL(LOG_LEVEL_INFO);

	set_physic_world(std::make_unique<physics::World>(1.5));

	auto width = game.get_graphics().get_width();
	auto height = game.get_graphics().get_height();

	create_ball(game, width, height);
	create_rocket(game, width, height);

	create_walls(width, height);
	load_level(game, width, height, current_level);

	game.get_audio().load_music("breakout", "audio/breakout.wav");
	game.get_audio().load_chunk("solid", "audio/solid.wav");

	game.get_audio().play_music("breakout");

	ball_->set_position(get_ball_start_pos());
}

void Simple_scene::update(Game &game) {
	//Scene::update(game);

	ball_->input(game);
	rocket_->input(game);

	if (!ball_->run_) {
		ball_->set_velosity(rocket_->get_velosity());
		ball_->set_direction(rocket_->get_direction());
		ball_->set_position(get_ball_start_pos());
	}

	for (auto& block : blocks) {
		block->update(game);
	}

	auto w = get_physic_world();
	blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&w](auto& block) {
		bool has_collision = block->get_body()->has_collision();
		bool is_solid = block->solid_;
		if (has_collision && !is_solid) {
			w->remove_body(block->get_body());
			return true;
		}
		return false;
	}), blocks.end());

	auto found_simple_blocks = std::find_if(blocks.begin(), blocks.end(), [](auto& block) {
		if (!block->solid_) {
			return true;
		}
		return false;
	});
	if (found_simple_blocks == blocks.end()) {
		next_level(game);
	}

	ball_->update(game);
	rocket_->update(game);
	get_physic_world()->step(0);
}

void Simple_scene::render(Game &game) { 
	ball_->render(game);
	rocket_->render(game);

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->render(game);
	}
}

