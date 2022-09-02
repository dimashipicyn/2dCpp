#ifndef FRAMERATE_H
#define FRAMERATE_H

#include "vector.h"

#include <stdint.h>

typedef struct s_graphics t_graphics;

void draw_number(t_graphics* graphics, t_point dest, int32_t number);
void draw_framerate(t_graphics* graphics, int32_t elapsed_time);

#endif

