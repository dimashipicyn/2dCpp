//
//  Sprite.cpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 11.09.2022.
//

#include "Sprite.h"

#include "game.h"
#include "graphics.h"


bool Sprite::load(Game &game, const std::string &file_name) {
    if (!texture_.load(game.get_graphics(), file_name)) {
        return false;
    }
    src_ = {0,0,texture_.get_w(), texture_.get_h()};
    dest_ = src_;
    return true;
}


void Sprite::render(Game &game) {
    Vec2f pos = Entity::get_position();
    dest_.x = pos.x;
    dest_.y = pos.y;
    
    game.get_graphics().draw_texture(texture_, src_, dest_);
}


void Sprite::update(Game &game) {
}


void Sprite::input(Game &game) {
}


Sprite::~Sprite() noexcept {
}


Sprite::Sprite()
    : Entity()
    , texture_()
    , src_()
    , dest_()
{
}

void Sprite::set_scale(float pt) {
    dest_.w *= pt;
    dest_.h *= pt;
}

int Sprite::get_height() const { 
    return dest_.h;
}


int Sprite::get_width() const { 
    return dest_.w;
}


float Sprite::get_scale() const { 
    return scale_;
}


void Sprite::set_height(int h) {
    dest_.h = h;
}


void Sprite::set_width(int w) {
    dest_.w = w;
}


