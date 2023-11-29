#include "texture.h"
#include "graphics.h"
#include "log.h"
#include "RaiiWrapper.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <SDL.h>
#include <png.h>

using SurfacePtr = std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)>;

Texture::Texture()
    : texture_(nullptr)
    , w_(0)
    , h_(0)
{
}

Texture::~Texture()
{
}

Texture::Texture(const Texture& texture)
    : texture_(texture.texture_)
    , w_(texture.w_)
    , h_(texture.h_)
{
}

bool Texture::load(Graphics& graphics, const std::string& file_name)
{
    png_image image; /* The control structure used by libpng */

    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_file(&image, file_name.c_str()) == 0)
    {
        return false;
    }

    image.format = PNG_FORMAT_RGBA;
    int imageSize = PNG_IMAGE_SIZE(image);

    RaiiWrapper<png_bytep, void(*)(void*), free> buffer((png_bytep)malloc(imageSize));
    if (buffer.get() != NULL && png_image_finish_read(&image, NULL /*background*/, buffer.get(), 0 /*row_stride*/, NULL /*colormap*/) != 0)
    {
        int pitch = imageSize / image.height;
        SurfacePtr surface { SDL_CreateSurfaceFrom(buffer.get(), image.width, image.height, pitch, SDL_PIXELFORMAT_RGBA32), SDL_DestroySurface };
        if (surface == nullptr)
        {
            LOG_ERROR("Could not create surface: " + file_name + ". Error: " + SDL_GetError());
            return false;
        }

        texture_.reset(SDL_CreateTextureFromSurface(graphics.renderer_, surface.get()), SDL_DestroyTexture);
        if (texture_ == nullptr)
        {
            LOG_ERROR("Could not create texture: " + file_name + ". Error: " + SDL_GetError());
            return false;
        }

        w_ = image.width;
        h_ = image.height;
    }

    return true;
}

int32_t Texture::get_w() const
{
    return w_;
}

int32_t Texture::get_h() const
{
    return h_;
}
