//
//  fonts.h
//  solong
//
//  Created by Дима Щипицын on 28/08/2022.
//

#ifndef fonts_h
#define fonts_h

#include "texture.h"

#include <stdint.h>

typedef struct _TTF_Font TTF_Font;

typedef struct s_font
{
	TTF_Font* font;
} t_font;

typedef struct s_color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} t_color;

typedef struct s_graphics t_graphics;

t_font	load_font(char* filename, int size);
void	free_font(t_font font);
t_texture	draw_text_to_texture(t_graphics* graphics, t_font font, char* text, t_color color);

#endif /* fonts_h */
