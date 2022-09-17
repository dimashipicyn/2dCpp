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

void Simple_scene::update(Game &game) {

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
}


Simple_scene::~Simple_scene() noexcept { 
}

void Simple_scene::render(Game &game) { 
	sprite_.draw(game.get_graphics());
	anim_sprite_.draw(game.get_graphics(), game.get_tick());
}

