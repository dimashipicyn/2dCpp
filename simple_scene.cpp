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

class SimpleBall : public Entity
{
public:
	SimpleBall() {
		square.x = 50;
		square.y = 50;
		square.w = 50;
		square.h = 50;
		set_position(Vec2f(50, 50));
		set_direction(Vec2f(0.2, 0.6));
		velocity = 0.5;
	};
	virtual ~SimpleBall() {};

	virtual void input(Game& game) override {};

	virtual void update(Game& game) override {
		auto size_w = game.get_graphics().get_width();
		auto size_h = game.get_graphics().get_height();
		auto pos = get_position();
		if (pos.x <= 0 || (pos.x + square.w) >= size_w) {
			auto dir = get_direction();
			dir.x = -dir.x;
			set_direction(dir);
		}
		if (pos.y <= 0 || (pos.y + square.h) >= size_h || !body->contacted_bodyes.empty()) {
			auto dir = get_direction();
			dir.y = -dir.y;
			if (!body->contacted_bodyes.empty()) {
				auto cont_body = body->contacted_bodyes.back();
				dir = dir.add(cont_body->direction.scalar(cont_body->velocity).normalize());
			}
			set_direction(dir);
		}
		translate(velocity * game.get_tick());
		body->aabb.pos = get_position();
	}

	virtual void render(Game& game) override {
		auto pos = get_position();
		auto dir = get_direction();
		pos = pos.add(dir.scalar(game.get_lag() * 1));
		square.x = pos.x;
		square.y = pos.y;
		game.get_graphics().draw_square(square, {0xFF,0,0,0xFF});
	}

	Rect square;
	float velocity;
	Physic_body* body;
};

class Rocket : public Entity
{
public:
	Rocket() {
		square.x = 300;
		square.y = 500;
		square.w = 100;
		square.h = 20;
		set_position(Vec2f(square.x, square.y));
		set_direction(Vec2f());
		velocity_ = 0.5;
	};
	virtual ~Rocket() {};

	virtual void input(Game& game) override {};

	virtual void update(Game& game) override {
		auto x_input = game.get_input().get_horizontal_axis();
		set_direction(Vec2f(x_input, 0).normalize());

//		if (x_input) {
//			velocity_ += 0.05;
//		}
//		else {
//			velocity_ -= 0.1;
//		}
//
//		velocity_ = velocity_ < 0 ? 0 : velocity_;
//		velocity_ = velocity_ > 2 ? 2 : velocity_;

		translate(velocity_ * game.get_tick());

		body->aabb.pos = get_position();
		body->direction = get_direction();
		body->velocity = velocity_;
	}

	virtual void render(Game& game) override {
		auto pos = get_position();
		auto dir = get_direction();
		pos = pos.add(dir.scalar(game.get_lag() * velocity_));
		square.x = pos.x;
		square.y = pos.y;
		game.get_graphics().draw_square(square, {0xFF,0xFF,0,0xFF});
	}

	Rect square;
	Physic_body* body;
	float velocity_;
};


void Simple_scene::update(Game &game) {
	physics_.step();
	ball_->update(game);
	rocket_->update(game);
}


void Simple_scene::start(Game &game) {
	SET_LOG_LEVEL(LOG_LEVEL_INFO);
	sprite_.load(game.get_graphics(), "assets/respawn_spritesheet.png");
	anim_sprite_.load(game.get_graphics(), "assets/respawn_spritesheet.png", 4);

	sprite_.set_position(50, 50);
	anim_sprite_.set_position(100, 100);
	anim_sprite_.set_duration_in_ms(500);
	anim_sprite_.set_repeat(true);
	anim_sprite_.set_scale(2);
	anim_sprite_.play();

	auto ball = std::make_unique<SimpleBall>();
	auto rocket = std::make_unique<Rocket>();

	Physic_body_def def;
	def.aabb = {Vec2f(ball->square.x, ball->square.y), Vec2f(ball->square.w, ball->square.h)};
	def.layer = Physic::LAYER_1;
	def.is_active = true;
	def.owner = ball.get();
	auto ballBody = physics_.create_body(def);
	ball->body = ballBody;

	def.aabb = {Vec2f(rocket->square.x, rocket->square.y), Vec2f(rocket->square.w, rocket->square.h)};
	def.layer = Physic::LAYER_1;
	def.is_active = true;
	def.owner = rocket.get();
	auto rocketBody = physics_.create_body(def);
	rocket->body = rocketBody;

	ball_ = std::move(ball);
	rocket_ = std::move(rocket);
}


Simple_scene::~Simple_scene() noexcept { 
}

void Simple_scene::render(Game &game) { 
	sprite_.draw(game.get_graphics());
	anim_sprite_.draw(game.get_graphics(), game.get_tick());
	ball_->render(game);
	rocket_->render(game);
	//game.get_graphics().draw_square({220,220,10,10}, {0x0,0xFF,0,0xFF});
}

