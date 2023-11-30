#pragma once

#include "Common.hpp"
#include "graphics.h"

#include <string>
#include <array>

class Game;

class Font;
using FontPtr = Ptr<Font>;

class TWODCPP_EXPORT Font
{
public:
    struct Glyph
    {
        SDL_Texture *texture = nullptr;
        Rect src = {};
        int bearingX = 0, bearingY = 0; // Offset from baseline to left/top of glyph
        int advanceX = 0, advanceY = 0; // Offset to advance to next glyph
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

    bool load(Game& game, const std::string& path, int fontsize);

    const Glyph& get_glyph(char c) const;
    const Size& get_size() const;
    Size get_str_size(const char* str) const;

private:
    friend class Graphics;
    SDL_Texture* texture = nullptr;
    std::array<Glyph, 256> glyphs;
    Size size;
};