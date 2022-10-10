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
		physics::AABB aabb(50,50,50,50);
		auto body = std::make_unique<physics::Body>(aabb);
		set_body(std::move(body));
		set_position(aabb.pos);
		set_direction(Vec2f(0, 1));
		velocity = 0.5;
		square.w = 50;
		square.h = 50;
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
		auto b = get_body();
		if (pos.y <= 0 || (pos.y + square.h) >= size_h || b->has_collision()) {
			auto dir = get_direction();
			dir.y = -dir.y;
//			if (!body->contacted_bodyes.empty()) {
//				auto cont_body = body->contacted_bodyes.back();
//				dir = dir.add(cont_body->direction.scalar(cont_body->velocity).normalize());
//			}
			set_direction(dir);
		}
		translate(velocity * game.get_tick());
	}

	virtual void render(Game& game) override {
		auto pos = get_position();
		auto dir = get_direction();
		pos = pos.add(dir.scalar(game.get_lag() * velocity));
		square.x = pos.x;
		square.y = pos.y;
		game.get_graphics().draw_square(square, {0xFF,0,0,0xFF});
	}

	Rect square;
	float velocity;
};

class Rocket : public Entity
{
public:
	Rocket() {
		physics::AABB aabb(300, 300, 100, 20);
		auto body = std::make_unique<physics::Body>(aabb);
		set_body(std::move(body));
		set_position(Vec2f(500, 500));
		set_direction(Vec2f());
		velocity_ = 0.5;
		square.w = 100;
		square.h = 20;
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
	float velocity_;
};


void Simple_scene::update(Game &game) {
	//Scene::update(game);

	ball_->update(game);
	rocket_->update(game);
	get_physic_world()->step(0);
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

	auto ball = std::make_shared<SimpleBall>();
	auto rocket = std::make_shared<Rocket>();

	auto ball_body = ball->get_body();
	auto rocket_body = rocket->get_body();

	ball_body->set_active(true);
	ball_body->set_layer(physics::Layer::LAYER_1);
	ball_body->set_owner(ball);

	rocket_body->set_active(true);
	rocket_body->set_layer(physics::Layer::LAYER_1);
	rocket_body->set_owner(rocket);

	auto w = std::make_unique<physics::World>(1.5);

	w->add_body(ball_body, true);
	w->add_body(rocket_body, true);
	set_physic_world(std::move(w));

	ball_ = std::move(ball);
	rocket_ = std::move(rocket);
}


Simple_scene::~Simple_scene() noexcept { 
}

void Simple_scene::render(Game &game) { 
//	sprite_.draw(game.get_graphics());
//	anim_sprite_.draw(game.get_graphics(), game.get_tick());
	ball_->render(game);
	rocket_->render(game);
	//game.get_graphics().draw_square({220,220,10,10}, {0x0,0xFF,0,0xFF});
}

