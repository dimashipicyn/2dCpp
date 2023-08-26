#ifndef GRAPHICS_H
# define GRAPHICS_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;

class Graphics;
class Sprite;
class Texture;
class Font;

struct Rect
{
	Rect(int32_t _x = 0, int32_t _y = 0, int32_t _w = 0, int32_t _h = 0)
		: x(_x)
		, y(_y)
		, w(_w)
		, h(_h)
	{}

	bool contains(int px, int py) const
	{
		if (px < x || (x + w) < px) {
            return false;
        }
        if (py < y || (y + h) < py) {
            return false;
        }
        return true;
	}

	bool intersect(const Rect& other) const
	{
        if ((x + w) < other.x || (other.x + other.w) < x) {
            return false;
        }
        if ((y + h) < other.y || (other.y + other.h) < y) {
            return false;
        }
        return true;
    }

	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
};

struct Color
{
	Color(uint8_t _r = 255, uint8_t _g = 255, uint8_t _b = 255, uint8_t _a = 255)
		: r(_r)
		, g(_g)
		, b(_b)
		, a(_a)
	{}

	Color operator+(const Color& o)
	{
		Color ret;
		ret.r = std::min(r + o.r, 255);
		ret.g = std::min(g + o.g, 255);
		ret.b = std::min(b + o.b, 255);
		ret.a = std::min(a + o.a, 255);

		return ret;
	}

	Color operator-(const Color& o)
	{
		Color ret;

		ret.r = std::max(r - o.r, 0);
		ret.g = std::max(g - o.g, 0);
		ret.b = std::max(b - o.b, 0);
		ret.a = std::max(a - o.a, 0);

		return ret;
	}

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

#define color_white Color(255,255,255,0)
#define color_red Color(255,0,0,0)
#define color_green Color(0,255,0,0)
#define color_blue Color(0,0,255,0)
#define rand_color Color(rand()%256,rand()%256,rand()%256,rand()%256)

class Graphics {
public:
    Graphics(int32_t width, int32_t height, const std::string& title);
    ~Graphics();

	Graphics(const Graphics& gr) = delete;

	bool is_ok();
    
    void render_frame();
    void clear_frame();
    
	void draw_texture(const Texture& texture, const Rect& src, const Rect& dest, const Color& color = Color());
	void draw_rect(const Rect& dest, const Color& color);
	void draw_outline_rect(const Rect& dest, const Color& color);
	void draw_pixel(int x, int y, const Color& color);
	void draw_line(int x1, int y1, int x2, int y2, const Color& color);
	void draw_char(const Font& font, int x, int y, char c, const Color& color);
	void draw_str(const Font& font, int x, int y, const char* str, const Color& color);

	void set_background_color(const Color& color);
    
    int32_t get_width() const;
    int32_t get_height() const;
    
private:
	friend class Texture;
	friend class Font;
    
    SDL_Window*		window_;
	SDL_Renderer*	renderer_;

    int32_t w_;
    int32_t h_;

	Color background_;

	bool ok_;
};

#endif
