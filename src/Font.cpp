#include "Font.h"
#include "game.h"
#include "RaiiWrapper.h"
#include "log.h"

#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <freetype/freetype.h>
#include <cassert>

using SurfacePtr = std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)>;

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

Font::Font() { }
Font::~Font()
{
    SDL_DestroyTexture(texture);
}

#define TRUNC(x) ((x) >> 6)

bool Font::load(Game& game, const std::string& path, int fontsize)
{
    FT_Error err = FT_Err_Ok;
    RaiiWrapper<FT_Library, FT_Error (*)(FT_Library), FT_Done_FreeType> ft;
    if (err = FT_Init_FreeType(&ft); err != FT_Err_Ok)
    {
        LOG_ERROR("Fail FT_Init_FreeType(). " + std::string(FT_Error_String(err)));
        return false;
    }

    RaiiWrapper<FT_Face, FT_Error (*)(FT_Face), FT_Done_Face> face;
    if (err = FT_New_Face(ft.get(), path.c_str(), 0, &face); err != FT_Err_Ok)
    {
        LOG_ERROR("Fail FT_New_Face(). " + std::string(FT_Error_String(err)));
        return false;
    }

    FT_Set_Pixel_Sizes(face.get(), 0, fontsize);

    const int surfaceSize = 512;
    SurfacePtr surface { SDL_CreateSurface(surfaceSize, surfaceSize, SDL_PIXELFORMAT_INDEX8), SDL_DestroySurface };
    if (!surface)
    {
        LOG_ERROR("Fail SDL_CreateSurface(). " + std::string(SDL_GetError()));
        return false;
    }

    SDL_SetSurfaceColorKey(surface.get(), SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    if (err = FT_Load_Char(face.get(), 'X', FT_LOAD_RENDER); err != FT_Err_Ok)
    {
        LOG_ERROR("Fail FT_Load_Char(). " + std::string(FT_Error_String(err)));
        return false;
    }

    size.w = face->glyph->bitmap.width;
    size.h = face->glyph->bitmap.rows;

    SDL_Rect dest = {};
    int offsetY = face->glyph->bitmap.rows + 10;
    for (int ch = 0; ch < 256; ch++)
    {
        if (err = FT_Load_Char(face.get(), ch, FT_LOAD_RENDER); err != FT_Err_Ok)
        {
            LOG_ERROR("Fail FT_Load_Char(). " + std::string(FT_Error_String(err)));
            return false;
        }

        SurfacePtr glyphSurface { SDL_CreateSurfaceFrom(
                                      face->glyph->bitmap.buffer,
                                      face->glyph->bitmap.width,
                                      face->glyph->bitmap.rows,
                                      face->glyph->bitmap.pitch,
                                      SDL_PIXELFORMAT_INDEX8),
            SDL_DestroySurface };
        if (glyphSurface == nullptr)
        {
            LOG_ERROR("Fail SDL_CreateSurfaceFrom(). " + std::string(SDL_GetError()));
            continue;
        }

        dest.w = face->glyph->bitmap.width;
        dest.h = face->glyph->bitmap.rows;
        if ((dest.x + dest.w) >= surfaceSize)
        {
            dest.x = 0;
            dest.y += offsetY;
            assert(dest.y < surfaceSize);
        }
        SDL_BlitSurface(glyphSurface.get(), NULL, surface.get(), &dest);

        glyphs[ch].bearingX = face->glyph->bitmap_left;
        glyphs[ch].bearingY = face->glyph->bitmap_top;
        glyphs[ch].advanceX = TRUNC(face->glyph->advance.x);
        glyphs[ch].advanceY = TRUNC(face->glyph->advance.y);
        glyphs[ch].src = {
            dest.x,
            dest.y,
            dest.w,
            dest.h
        };

        dest.x += dest.w;
    }

    texture = SDL_CreateTextureFromSurface(game.get_graphics().renderer_, surface.get());
    for (int ch = 0; ch < 256; ch++)
    {
        glyphs[ch].texture = texture;
    }

    return true;
}

const Font::Glyph& Font::get_glyph(char c) const
{
    return glyphs[uint8_t(c)];
}

Font::Size Font::get_str_size(const char* str) const
{
    Size s;
    for (const char* c = str; *c != '\0'; c++)
    {
        Glyph gl = get_glyph(*c);
        s.w += gl.advanceX;
        s.h = std::max(gl.src.h, s.h);
    }
    return s;
}

const Font::Size& Font::get_size() const
{
    return size;
}
