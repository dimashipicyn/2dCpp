#include "stdafx.h"

#include "texture.h"
#include "graphics.h"
#include "log.h"

#include <stddef.h>
#include <assert.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>

Texture::Texture()
	: texture_(nullptr)
	, w_(0)
	, h_(0)
{
}

Texture::~Texture()
{
}

bool Texture::load(Graphics& graphics, const std::string &file_name)
{
    std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> image{IMG_Load(file_name.c_str()), SDL_FreeSurface};
	if (image == nullptr) {
        LOG_ERROR("Could not load texture: " + file_name + ". Error: " + SDL_GetError());
		return false;
	}
	SDL_SetColorKey(image.get(), SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0x01));

    //texture_.reset(SDL_CreateTextureFromSurface(graphics.renderer_, image.get()), SDL_DestroyTexture);
    if (texture_ == nullptr) {
        LOG_ERROR("Could not create texture: " + file_name + ". Error: " + SDL_GetError());
        return false;
    }
    
	w_ = image->w;
	h_ = image->h;

    return true;
}

int32_t Texture::get_w() const {
    return w_;
}

int32_t Texture::get_h() const {
    return h_;
}
