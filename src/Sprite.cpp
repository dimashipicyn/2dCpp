//
//  Sprite.cpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 11.09.2022.
//

#include "stdafx.h"

#include "Sprite.h"

#include "game.h"
#include "graphics.h"
#include "log.h"

Sprite::Sprite()
: texture_()
, src_()
, dest_()
, scale_(1)
{
}

Sprite::Sprite(const Sprite& s)
: texture_(s.texture_)
, src_(s.src_)
, dest_(s.dest_)
, scale_(s.scale_)
{

}

Sprite::Sprite(const Texture& texture)
: texture_(texture)
, src_(0,0,texture.get_w(), texture.get_h())
, dest_(0,0,texture.get_w(), texture.get_h())
, scale_(1)
{

}

Sprite::Sprite(const Texture& texture, const Rect& src, const Rect& dest)
    : texture_(texture)
    , src_(src)
    , dest_(dest)
    , scale_(1)
{
}

Sprite::~Sprite() noexcept {
}

bool Sprite::load(Graphics& graphics, const std::string &file_name) {
    if (!texture_.load(graphics, file_name)) {
        return false;
    }
    src_ = {0,0,texture_.get_w(), texture_.get_h()};
    dest_ = src_;
    return true;
}

void Sprite::set_scale(float pt) {
    dest_.w *= pt;
    dest_.h *= pt;
}

int Sprite::get_x() const
{
	return dest_.x;
}

int Sprite::get_y() const
{
	return dest_.y;
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

bool Sprite::is_intersect(const Sprite& other)
{
	return dest_.intersect(other.dest_);
}

void Sprite::draw(Graphics &graphics) {
    graphics.draw_texture(texture_, src_, dest_, color_);
}

void Sprite::set_position(int x, int y) {
    dest_.x = x;
    dest_.y = y;
}

void Sprite::set_size(int w, int h) {
    dest_.w = w;
    dest_.h = h;
}

void Sprite::set_color(const Color& color) {
	color_ = color;
}
