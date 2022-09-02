//
//  fonts.c
//  solong
//
//  Created by Дима Щипицын on 28/08/2022.
//

#include <SDL.h>
#include <SDL_ttf.h>

#include "fonts.h"
#include "graphics.h"

t_font load_font(char* fontname, int size)
{
	t_font font;
	font.font = TTF_OpenFont(fontname, size);
	if (font.font == NULL) {
		printf("Dont open font: %s!\n", fontname);
	}
	return font;
}

t_texture draw_text_to_texture(t_graphics* graphics, t_font font, char* text, t_color color)
{
	t_texture font_texture = {NULL, 0, 0};

	SDL_Color text_color = {color.r, color.g, color.b, color.a};
    SDL_Surface* txt_surf = TTF_RenderText_Solid(font.font , text, text_color);
    if (txt_surf) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics->renderer, txt_surf);
        if (texture) {
            font_texture.texture = texture;
            font_texture.w = txt_surf->w;
            font_texture.h = txt_surf->h;
        }
        SDL_FreeSurface(txt_surf);
    }
    return font_texture;
}
