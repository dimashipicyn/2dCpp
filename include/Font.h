#pragma once

#include "graphics.h"
#include "Common.hpp"

#include <string>
#include <array>

class Font;
using FontPtr = Ptr<Font>;

class Font
{
public:
    struct Glyph
    {
        SDL_Texture *texture;
        Rect src;
    };

    struct Size
    {
        int w = 0;
        int h = 0;
    };

    Font();
    ~Font();

    Font(const Font& o) = delete;
    Font(Font&& o) = delete;

    bool load(Graphics& graphics, const std::string& path, int fontsize);

    Glyph get_glyph(char c) const;
    Size get_str_size(const char* str);

private:
    SDL_Texture* texture = nullptr;
    std::array<Rect, 256> glyphs;
};