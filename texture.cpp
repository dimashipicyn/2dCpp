#include "texture.h"
#include "graphics.h"

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
	SDL_DestroyTexture(texture_);
	texture_ = nullptr;
}

void Texture::load(Graphics& graphics, const std::string &file_name)
{
	SDL_Surface* image = IMG_Load(file_name.c_str());
	if (image == NULL) {
		printf("Could not load texture: %s\n", file_name.c_str());
		return ;
	}
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0, 0x01));

	texture_ = SDL_CreateTextureFromSurface(graphics.renderer_, image);
	w_ = image->w;
	h_ = image->h;

	SDL_FreeSurface(image);
}
