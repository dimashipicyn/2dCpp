//
//  animation.c
//  ft
//
//  Created by Dmitry Shipicyn on 30.06.2022.
//

#include "animation.h"
#include "graphics.h"

#include <math.h>

t_animation animation(t_sprite sprite, int32_t nframes, int32_t duration, int8_t repeat, int8_t is_play)
{
    t_animation a = {sprite, nframes, duration, 0, 0, repeat, 0, is_play};
    
    return a;
}

void update_animation(t_animation* animation, int32_t elapsed_time)
{
	int8_t is_playing = (!animation->is_end && animation->is_play);
	is_playing = is_playing || (animation->repeat && animation->is_play);
	if (is_playing) {
		int32_t index = animation->nframes * animation->elapsed / animation->duration % animation->nframes;
		index = animation->repeat ? index % animation->nframes : fmin(index, animation->nframes - 1);
    
		animation->index = index;
	}
    
    if (animation->elapsed >= animation->duration) {
        animation->is_end = 1;
    }
	animation->elapsed += elapsed_time;
}

t_sprite get_animation_sprite(t_animation* animation)
{
    t_sprite s = animation->sprite;

    s.src.pos.x = s.src.pos.x + (s.src.size.x * animation->index);
    
    return s;
}

void play_animation(t_animation* animation)
{
	animation->is_play = 1;
}

void stop_animation(t_animation* animation)
{
	animation->is_play = 0;
}
