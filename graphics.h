#ifndef GRAPHICS_H
# define GRAPHICS_H

#include "vector.h"
#include "texture.h"

#include <cstdint>
#include <memory>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;

class Graphics {
    Graphics();
    ~Graphics();
private:
    std::unique_ptr<SDL_Window, void(SDL_Window*)>		window_;
	std::unique_ptr<SDL_Renderer, void(SDL_Renderer*)>	renderer;
    int32_t         w;
    int32_t         h;
};

typedef struct s_line {
    t_vec2 a;
    t_vec2 b;
} t_line;

typedef struct s_rect {
	t_vec2 pos;
	t_vec2 size;
} t_rect;

typedef struct s_circle {
    t_vec2 center;
    float radius;
} t_circle;

typedef struct s_triangle {
    t_vec2 a;
    t_vec2 b;
    t_vec2 c;
} t_triangle;

typedef struct s_sprite {
    t_texture   texture;
    t_rect      dest;
    t_rect      src; 
} t_sprite;

typedef struct s_game_ctx t_game_ctx;

t_graphics*	init_graphics(int width, int height, char* title);
void		draw_sprite_to_frame(t_graphics* graphics, t_sprite sprite);
void		render_frame(t_graphics* graphics);
void		clear_frame(t_graphics* graphics);
void        draw_line_to_frame(t_graphics* grapics, t_line line);
void        draw_rect_to_frame(t_graphics* grapics, t_rect rect);
void        draw_circle_to_frame(t_graphics* grapics, t_circle circle);
void        draw_triangle_to_frame(t_graphics* grapics, t_triangle triangle);

#endif
