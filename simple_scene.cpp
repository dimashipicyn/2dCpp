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

void Simple_scene::update(Game &game) {
//    sprite_->translate(2);
//
//    Vec2f pos = sprite_->get_position();
//
//    int x_max = game.get_graphics().get_width();
//    int y_max = game.get_graphics().get_height();
//
//    int sprite_w = sprite_->get_width();
//    int sprite_h = sprite_->get_height();
//
//    if (pos.x > x_max - sprite_w) {
//        sprite_->translate(-2);
//        sprite_->rotate(-90);
//    }
//    if (pos.y > y_max - sprite_h) {
//        sprite_->translate(-2);
//        sprite_->rotate(-90);
//    }
//    if (pos.x < 0) {
//        sprite_->translate(-2);
//        sprite_->rotate(-90);
//    }
//    if (pos.y < 0) {
//        sprite_->translate(-2);
//        sprite_->rotate(-90);
//    }
}


void Simple_scene::start(Game &game) {
//    sprite_ = std::make_shared<Sprite>();
//    sprite_->load(game, "assets/orel_spritesheet.png");
//    sprite_->set_scale(5);
//    sprite_->set_direction(Vec2f(1,0));
//    left = true;
//    attach_entity(sprite_);
//    
//    Sprite_atlas atlas;
//    atlas.load(game, "assets/test.json");
//    auto s = atlas.get("wall_3");
//    s->set_position(Vec2f(100,100));
//    attach_entity(s);
}


Simple_scene::~Simple_scene() noexcept { 
}
