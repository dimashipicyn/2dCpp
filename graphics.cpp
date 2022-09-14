#include "graphics.h"

#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

Graphics::Graphics(int32_t width, int32_t height, const std::string &title)
    : window_(nullptr)
    , renderer_(nullptr)
    , w_(width)
    , h_(height)
	, ok_(true)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		ok_ = false;
		printf( "SDL video could not initialize! Error: %s\n", SDL_GetError());
	}

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init( imgFlags ) & imgFlags))
    {
		ok_ = false;
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    if(TTF_Init() == -1)
    {
		ok_ = false;
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }
    
    window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!window_ || !renderer_) {
		ok_ = false;
		printf("SDL could not initialize! %s\n", SDL_GetError());
	}

	SDL_GetWindowSize(window_, &w_, &h_);
}

Graphics::~Graphics()
{
	if (renderer_) {
		SDL_DestroyRenderer(renderer_);
		renderer_ = nullptr;
	}
	if (window_) {
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Graphics::render_frame()
{
	SDL_RenderPresent(renderer_);
}

void Graphics::clear_frame()
{
	SDL_RenderClear(renderer_);
}

bool Graphics::is_ok() { 
	return ok_;
}


void Graphics::draw(const Drawable* object)
{
	object->draw(*this);
}



Drawable::~Drawable() {
}


void Graphics::draw_texture(const Texture &texture, const Rect &src, const Rect &dest) {
	SDL_Rect s{src.x, src.y, src.w, src.h};
	SDL_Rect d{dest.x, dest.y, dest.w, dest.h};
	SDL_RenderCopy(renderer_, texture.texture_, &s, &d);
}


int32_t Graphics::get_height() const {
    return h_;
}


int32_t Graphics::get_width() const { 
    return w_;
}
