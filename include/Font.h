#pragma once

#include "Export.h"
#include "graphics.h"
#include "Common.hpp"

#include <string>
#include <array>

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

    bool load(Graphics& graphics, const std::string& path, int fontsize);

    const Glyph& get_glyph(char c) const;
    const Size& get_str_size(const char* str) const;
    const Size& get_size() const;

private:
    friend class Graphics;
    SDL_Texture* texture = nullptr;
    std::array<Glyph, 256> glyphs;
    Size size;
};