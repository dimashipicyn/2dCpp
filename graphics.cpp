#include "game.h"
#include "graphics.h"
#include "texture.h"

#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#if 0
static inline int get_pixel_texture(t_texture* texture, int x, int y)
{
    register int *color = (int*)((uint8_t*)texture->addr + (x * texture->bits_per_pixel)
                                     + (y * texture->width * texture->bits_per_pixel)
                                     );
    return *color;
}

static inline void set_pixel_texture(t_texture *texture, int x, int y, int color)
{
    *(int *)((uint8_t*)texture->addr + (x * 4)
                           + (y * texture->width * 4)
                           ) = color;
}

void draw_sprite_to_frame(t_graphics* graphics, t_sprite sprite)
{
    t_texture* frame = graphics->frame;

	t_rect dest = sprite.dest;
	t_rect src = sprite.src;

    int x_max = (dest.pos.x + dest.size.x) < frame->width ? dest.size.x : frame->width - dest.pos.x;
    int y_max = (dest.pos.y + dest.size.y) < frame->height ? dest.size.y : frame->height - dest.size.y;

    int x_min = dest.pos.x >= 0 ? 0 : -dest.pos.x;
    int y_min = dest.pos.y >= 0 ? 0 : -dest.pos.y;
    
    if (x_min < 0 || y_min < 0 || x_max < 0 || y_max < 0) {
        return;
    }

    float scale_x = src.size.x / dest.size.x;
    float scale_y = src.size.y / dest.size.y;

    for (int y = y_min; y < y_max; ++y) {

        for (int x = x_min; x < x_max; ++x) {

            // получаем нужный пиксель с учетом масштаба
            int color = get_pixel_texture(sprite.texture, src.pos.x + x * scale_x, src.pos.y + y * scale_y);

            // черный цвет пропускаем
            if (/* color == 0x00FFFFFF || */ color == 0x00000001) {
                continue;
            }
            
            set_pixel_texture(frame, x + dest.pos.x, y + dest.pos.y, color);
        }
    }
}

void clear(t_graphics* graphics)
{
    mlx_clear_window(graphics->mlx, graphics->window);
}

void render_frame(t_graphics* graphics)
{
    mlx_put_image_to_window(graphics->mlx, graphics->window, graphics->frame->image, 0, 0);
    mlx_do_sync(graphics->mlx);
}

t_graphics *init_graphics(int width, int height, char* title)
{
    void*       mlx;
    void*       window;
    void*       addr;
    void*       image;

    mlx = mlx_init();
    if (!mlx) {
        return NULL;
    }

    window = mlx_new_window(mlx, width, height, title);
    if (!window) {
        return NULL;
    }

    image = mlx_new_image(mlx, width, height);
    if (!image) {
        mlx_destroy_window(mlx, window);
    }

    int endian;
    int size_line;
    int bits_per_pixel;
    addr = mlx_get_data_addr(image, &bits_per_pixel, &size_line, &endian);

    t_texture *frame = calloc(1, sizeof(t_texture));
    *frame = (t_texture) {
        .image = image,
        .addr = addr,
        .bits_per_pixel = bits_per_pixel,

        .width = width,
        .height = height
    };

    t_graphics  *graphics = calloc(1, sizeof(t_graphics));

    *graphics = (t_graphics) {
        .mlx = mlx,
        .window = window,

        .frame = frame
    };

    return graphics;
}

#endif

t_graphics *init_graphics(int width, int height, char* title)
{
    SDL_Window*		window = NULL;
	SDL_Renderer*	renderer = NULL;
    
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!( IMG_Init( imgFlags ) & imgFlags ))
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return NULL;
    }
    
    //Initialize SDL_ttf
    if(TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        return NULL;
    }

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_DestroyWindow(window);
		return NULL;
	}
	SDL_SetRenderDrawColor(renderer, 0x3F, 0x3F, 0x3F, 0xFF);

    t_graphics  *graphics = calloc(1, sizeof(t_graphics));

    *graphics = (t_graphics) {
        .window = window,
		.renderer = renderer,
        .w = width,
        .h = height
    };

    return graphics;
}

void render_frame(t_graphics* graphics)
{
	SDL_RenderPresent(graphics->renderer);
}

void clear_frame(t_graphics* graphics)
{
	SDL_RenderClear(graphics->renderer);
}

void draw_sprite_to_frame(t_graphics* graphics, t_sprite sprite)
{
	SDL_Rect src = {sprite.src.pos.x, sprite.src.pos.y, sprite.src.size.x, sprite.src.size.y};
	SDL_Rect dest = {sprite.dest.pos.x, sprite.dest.pos.y, sprite.dest.size.x, sprite.dest.size.y};
	SDL_RenderCopy(graphics->renderer, sprite.texture.texture, &src, &dest);
}
