//
//  AnimationSprite.cpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 16.09.2022.
//

#include "AnimationSprite.hpp"

AnimationSprite::AnimationSprite()
    : Sprite()
    , animation_list_()
    , frame_index_(0)
    , duration_in_ms_(0)
    , playing_(false)
{
}

AnimationSprite::AnimationSprite(const Texture &texture, const std::vector<Rect> &src_list, const Rect &dest)
    : Sprite(texture, Rect{}, dest)
    , animation_list_(src_list)
{
    
}

AnimationSprite::~AnimationSprite() {
}

void AnimationSprite::stop() {
    playing_ = false;
}


void AnimationSprite::play() {
    playing_ = true;
}

void AnimationSprite::draw(Graphics &graphics) {
    if (playing_) {
        Rect src = animation_list_[frame_index_];
        graphics.draw_texture(texture_, src, dest_);
    }
}

bool AnimationSprite::load(Graphics &graphics, const std::string &path_to_texture, int frames_in_animation) {
    if (!Sprite::load(graphics, path_to_texture)) {
        return false;
    }
    Rect src_copy = src_;
    for (int i = 0; i < frames_in_animation; i++) {
        src_copy.x += src_copy.w;
        animation_list_.push_back(src_copy);
    }
    
    return true;
}

void AnimationSprite::set_duration_in_ms(int ms) { 
    duration_in_ms_ = ms;
}






