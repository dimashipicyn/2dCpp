#include "Font.h"

#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <cassert>
#include <log.h>

// BitmapFont::BitmapFont(Graphics& graphics, const std::string& path)
// {
//     font_ = TTF_OpenFont(path.c_str(), 20);
// }

// BitmapFont::~BitmapFont()
// {
//     TTF_CloseFont(font_);
// }

// void BitmapFont::draw_str(Graphics& graphics, int x, int y, const std::string& str, int size, const Color& color)
// {
//     SDL_Color text_color = {color.r, color.g, color.b, color.a};
//     SDL_Surface* txt_surf = TTF_RenderText_Solid(font_, str.c_str(), text_color);
//     SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.renderer_, txt_surf);

//     SDL_Rect src = {0, 0, txt_surf->w, txt_surf->h};
//     SDL_Rect dest = {x, y, txt_surf->w, txt_surf->h};
//     SDL_RenderCopy(graphics.renderer_, texture, &src, &dest);

//     SDL_FreeSurface(txt_surf);
//     SDL_DestroyTexture(texture);
// }

// BitmapFont::BitmapFont(SDL_Renderer* renderer, const char* filename, int charWidth, int charHeight)
// {
//     SDL_Surface* fontSurface = SDL_LoadBMP(filename);
//     fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
//     this->charWidth = fontSurface->w / charWidth;
//     this->charHeight = fontSurface->h / charHeight;
//     SDL_FreeSurface(fontSurface);
// }

// void BitmapFont::renderChar(SDL_Renderer* renderer, char c, int x, int y)
// {
//     int charX, charY;
//     getCharPos(c, &charX, &charY);
//     SDL_Rect srcRect = {charX * charWidth, charY * charHeight, charWidth, charHeight};
//     SDL_Rect destRect = {x, y, charWidth, charHeight};
//     SDL_RenderCopy(renderer, fontTexture, &srcRect, &destRect);
// }

// BitmapFont::~BitmapFont()
// {
//     SDL_DestroyTexture(fontTexture);
// }

Font::Font() {}
Font::~Font() {
    SDL_DestroyTexture(texture);
}

bool Font::load(Graphics& graphics, const std::string &path, int fontsize) {
    /*TTF_Font* font = TTF_OpenFont(path.c_str(), fontsize);
    if (!font)
    {
        LOG_ERROR("Could not load font: " + path);
        return false;
    }

    SDL_Surface *surface = nullptr;
    SDL_Surface *text = nullptr;
	SDL_Rect dest = {};
	char c[2] = {};

    const int size = 512;

    surface = SDL_CreateSurface(size, size, SDL_PIXELFORMAT_RGBA8888);
    if (!surface)
    {
        LOG_ERROR("Could not create surface: ");
        TTF_CloseFont(font);
        return false;
    }

	SDL_SetSurfaceColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    for (int i = ' ' ; i < 128 ; i++)
	{
        c[0] = i;

        text = TTF_RenderUTF8_Blended(font, c, SDL_Color{0xFF,0xFF,0xFF,0xFF});
        if (!text)
        {
            LOG_ERROR("Could not create surface: ");
            TTF_CloseFont(font);
            SDL_DestroySurface(surface);
            return false;
        }
        
        TTF_SizeText(font, c, &dest.w, &dest.h);

        if (dest.x + dest.w >= size)
        {
            dest.x = 0;
            dest.y = dest.y + dest.h;
            assert(dest.y < size);
        }

        SDL_BlitSurface(text, NULL, surface, &dest);
        
        glyphs[i].x = dest.x;
        glyphs[i].y = dest.y;
        glyphs[i].w = dest.w;
        glyphs[i].h = dest.h;

        dest.x = dest.x + dest.w;

        SDL_DestroySurface(text);
    }

    texture = SDL_CreateTextureFromSurface(graphics.renderer_, surface);

    TTF_CloseFont(font);
    SDL_DestroySurface(surface);*/

    return true;
}

Font::Glyph Font::get_glyph(char c) const
{
    return Glyph{texture, glyphs[uint8_t(c)]};
}

Font::Size Font::get_str_size(const char* str)
{
    Size s;
    for (const char* c = str; *c != '\0'; c++)
    {
        Glyph gl = get_glyph(*c);
        s.w += gl.src.w;
        s.h = std::max(gl.src.h, s.h);
    }
    return s;
}