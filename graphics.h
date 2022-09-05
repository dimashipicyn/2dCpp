#ifndef GRAPHICS_H
# define GRAPHICS_H

#include "vector.h"
#include "texture.h"

#include <cstdint>
#include <memory>
#include <string>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;

class Graphics {
public:
    Graphics(int32_t width, int32_t height, const std::string& title);
    ~Graphics();
    
    //void draw_sprite_to_frame(const Sprite& sprite);
    void render_frame();
    void clear_frame();
    
    
private:
    struct WindowDeleter {
        void operator()(SDL_Window* window);
    };
    struct RendererDeleter {
        void operator()(SDL_Renderer* renderer);
    };
    
    std::unique_ptr<SDL_Window, WindowDeleter>		window_;
	std::unique_ptr<SDL_Renderer, RendererDeleter>	renderer_;
    int32_t w_;
    int32_t h_;
};

#endif
