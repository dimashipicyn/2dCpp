//
//  animation.h
//  solong
//
//  Created by Dmitry Shipicyn on 30.06.2022.
//

#ifndef animation_h
#define animation_h

#include "graphics.h"

#include <stdint.h>

typedef struct s_animation
{
    t_sprite sprite;
    int32_t nframes;
    int32_t duration;
    int32_t elapsed;
    uint8_t index;
    int8_t	repeat;
    int8_t	is_end;
	int8_t	is_play;
} t_animation;

t_animation animation(t_sprite sprite, int32_t nframes, int32_t duration, int8_t repeat, int8_t is_play);
void        update_animation(t_animation* animation, int32_t elapsed_time);
t_sprite    get_animation_sprite(t_animation* animation);
void		play_animation(t_animation* animation);
void		stop_animation(t_animation* animation);

#endif /* animation_h */
