#include "graphics.h"

#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

void Graphics::WindowDeleter::operator()(SDL_Window* window)
{
    if (window) {
        SDL_DestroyWindow(window);
    }
}

void Graphics::RendererDeleter::operator()(SDL_Renderer* renderer)
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
}

Graphics::Graphics(int32_t width, int32_t height, const std::string &title)
    : window_(nullptr, WindowDeleter())
    , renderer_(nullptr, RendererDeleter())
    , w_(width)
    , h_(height)
{
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!( IMG_Init( imgFlags ) & imgFlags ))
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
    
    //Initialize SDL_ttf
    if(TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    
    window_ = std::unique_ptr<SDL_Window, void(SDL_Window*)>(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN), SDL_DestroyWindow);
    
    renderer_ = std::unique_ptr<SDL_Renderer, void(SDL_Renderer*)>(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer);
}

Graphics::~Graphics()
{
    
}

void Graphics::render_frame()
{
	SDL_RenderPresent(renderer_.get());
}

void Graphics::clear_frame()
{
	SDL_RenderClear(renderer_.get());
}

//void Graphics::draw_sprite(const Sprite& sprite)
//{
//    SDL_Rect src = {sprite.src.pos.x, sprite.src.pos.y, sprite.src.size.x, sprite.src.size.y};
//    SDL_Rect dest = {sprite.dest.pos.x, sprite.dest.pos.y, sprite.dest.size.x, sprite.dest.size.y};
//    SDL_RenderCopy(graphics->renderer, sprite.texture.texture, &src, &dest);
//}

