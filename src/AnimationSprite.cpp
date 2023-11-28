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
	, elapsed_in_ms_(0)
    , playing_(false)
	, repeat_(false)
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

void AnimationSprite::draw(Graphics &graphics, int elapsed_in_ms) {
    if (playing_) {
		int frames = animation_list_.size();
		frame_index_ = (frames * elapsed_in_ms_) / duration_in_ms_;
		if (repeat_) {
			frame_index_ %= frames;
		}
		else {
			frame_index_ = std::min(frame_index_, frames - 1);
		}
    }
	elapsed_in_ms_ += elapsed_in_ms;
	Rect src = animation_list_[frame_index_];
	graphics.draw_texture(texture_, src, dest_);
}

bool AnimationSprite::load(Graphics &graphics, const std::string &path_to_texture, int frames_in_animation) {
    if (!Sprite::load(graphics, path_to_texture)) {
        return false;
    }
    Rect src_copy = src_;
	src_copy.w = src_.w / frames_in_animation;
    for (int i = 0; i < frames_in_animation; i++) {
        animation_list_.push_back(src_copy);
		src_copy.x += src_copy.w;
    }
	dest_.w = src_copy.w;
	dest_.h = src_copy.h;
    
    return true;
}

void AnimationSprite::set_duration_in_ms(int ms) { 
    duration_in_ms_ = ms;
}

void AnimationSprite::set_repeat(bool val) {
	repeat_ = val;
}

bool AnimationSprite::is_end() const { 
	if (elapsed_in_ms_ >= duration_in_ms_) {
		return true;
	}
	return false;
}








