#include "game.h"
#include "utils.h"
#include "tank.h"
#include "vector.h"
#include "framerate.h"
#include "game_map.h"
#include "settings.h"
#include "main_scene.h"
#include "menu_scene.h"
#include "editor_scene.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <stdio.h>



int close_game() {
    exit(0);
}

void loop(t_game_ctx* game)
{
    
}

char* get_current_dir(char** env)
{
    while (*env)
    {
        if (strncmp(*env, "PWD", 3) == 0) {
            char* s = strchr(*env, '=');
            return ++s;
        }
        env++;
    }
	return "";
}

t_game_ctx* init_game(char** env)
{
    t_game_ctx* game = NULL;
    t_graphics* graphics = NULL;
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Could not initialize video or audio.\n");
        return NULL;
    }

    game = calloc(1, sizeof(t_game_ctx));
    if (!game) {
        printf("Could not initialize game. Bad alloc.\n");
        return NULL;
    }
    
    char* path_to_cur_dir = get_current_dir(env);
    
    char buf[256];
    strlcpy(buf, path_to_cur_dir, 256);
    strlcat(buf, "/settings.yml", 256);
    if (load_settings(buf)) {
        goto error;
    }
    
    t_settings* s = get_settings();
    graphics = init_graphics(500, 448, s->game_name);
    if (!graphics) {
        printf("Could not initialize graphics.\n");
        goto error;
    }

    if (load_textures(graphics, path_to_cur_dir)) {
        printf("Loading textures failed.\n");
        goto error;
    }
    
    game->graphics = graphics;

	game->scenes[MAIN_SCENE] = new_main_scene();
	game->scenes[MENU_SCENE] = new_menu_scene();
	game->scenes[EDITOR_SCENE] = new_editor_scene();

	for (int i = 0; i < TOTAL_SCENES; i++) {
		scene_preload(game->scenes[i], game);
	}

	game->active_scene = game->scenes[MENU_SCENE];
	
    return game;

error:
    free(game);
    free(graphics);

    return NULL;
}

int main(int ac, char** argv, char** env) {
	(void)ac;
	(void)argv;

    t_game_ctx* game = init_game(env);
    if (!game) {
        printf("Could not init game!\n");
        return 1;
    }

    loop(game);

    return 0;
}
