#include "utils.h"

#include "texture.h"
#include "graphics.h"
#include "game.h"

#include <stddef.h>
#include <assert.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>

static t_texture textures[TOTAL_TEXTURES];

t_texture get_texture(t_texture_id id)
{
	assert(id >= 0 && id < TOTAL_TEXTURES);
    return textures[id];
}

static char *texture_filenames[TOTAL_TEXTURES] = {
	[TANK_YELLOW_TXR_ID] = "/assets/tank_yellow_spritesheet.png",
	[TANK_GREEN_TXR_ID]  = "/assets/tank_green_spritesheet.png",
	[TANK_RED_TXR_ID]    = "/assets/tank_red_spritesheet.png",
	[TANK_WHITE_TXR_ID]  = "/assets/tank_white_spritesheet.png",
	[BULLET_TXR_ID]      = "/assets/bullet_spritesheet.png",
	[TERRAIN_TXR_ID]     = "/assets/terrain_spritesheet.png",
	[RESPAWN_TXR_ID]     = "/assets/respawn_spritesheet.png",
	[EFFECTS_TXR_ID]     = "/assets/effects_spritesheet.png",
	[BONUSES_TXR_ID]     = "/assets/bonuses_spritesheet.png",
	[OREL_TXR_ID]        = "/assets/orel_spritesheet.png",
	[DIRT_TXR_ID]        = "/assets/dirt.png",
	[DIGITS_TXR_ID]      = "/assets/digits_spritesheet.png"
};

void set_texture_filename(char* filename, t_texture_id id)
{
	assert(id >= 0 && id < TOTAL_TEXTURES);
	texture_filenames[id] = filename;
}

enum { MAX_SIZE_PATH = 256 };

int load_textures(t_graphics* ctx, char* path_to_cur_dir)
{
	char filename[MAX_SIZE_PATH];
	int txr_id = 0;
	while (txr_id < TOTAL_TEXTURES)
	{
		memset(filename, 0, MAX_SIZE_PATH);
		strlcat(filename, path_to_cur_dir, MAX_SIZE_PATH);
		strlcat(filename, texture_filenames[txr_id], MAX_SIZE_PATH);

		SDL_Surface* image = IMG_Load(filename);
		if (image == NULL) {
			printf("Could not load texture: %s\n", filename);
			goto error;
		}
		SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0x01));

		t_texture texture;
		texture.texture = SDL_CreateTextureFromSurface(ctx->renderer, image);
		SDL_FreeSurface(image);
		
		textures[txr_id] = texture;
		txr_id++;
	}

	return 0;

error:
	for (int i = 0; i < TOTAL_TEXTURES; i++) {
		if (textures[i].texture) {
			SDL_DestroyTexture(textures[i].texture);
		}
	}
	return 1;
}

t_texture load_texture(char* filename, t_graphics* ctx)
{
	t_texture texture = {NULL, 0, 0};

	SDL_Surface* image = IMG_Load(filename);
	if (image == NULL) {
		printf("Could not load texture: %s\n", filename);
		return texture;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0x01));

	texture.texture = SDL_CreateTextureFromSurface(ctx->renderer, image);
	texture.w = image->w;
	texture.h = image->h;
	
	SDL_FreeSurface(image);
	return texture;
}

void destroy_texture(t_texture texture)
{
	SDL_DestroyTexture(texture.texture);
}
