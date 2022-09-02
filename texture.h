#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

typedef enum texture_id {
	TANK_YELLOW_TXR_ID,
	TANK_GREEN_TXR_ID,
	TANK_RED_TXR_ID,
	TANK_WHITE_TXR_ID,
	BULLET_TXR_ID,
	TERRAIN_TXR_ID,
	RESPAWN_TXR_ID,
	EFFECTS_TXR_ID,
	BONUSES_TXR_ID,
	OREL_TXR_ID,
	DIRT_TXR_ID,
	DIGITS_TXR_ID,
    TOTAL_TEXTURES
} t_texture_id;

typedef struct SDL_Texture SDL_Texture;

typedef struct s_texture {
	SDL_Texture* texture;
	int32_t w;
	int32_t h;
} t_texture;

typedef struct s_graphics t_graphics;

int load_textures(t_graphics* ctx, char* path_to_cur_dir);
t_texture get_texture(t_texture_id id);
void set_texture_filename(char* filename, t_texture_id id);

t_texture load_texture(char* filename, t_graphics* ctx);
void destroy_texture(t_texture texture);

#endif
