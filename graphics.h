#ifndef GRAPHICS_H
# define GRAPHICS_H

#include "vector.h"
#include "texture.h"

#include <cstdint>
#include <memory>
#include <string>
#include <glm/glm.hpp>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;
typedef void* SDL_GLContext;

class Graphics;

class Drawable
{
public:
	virtual ~Drawable();

	virtual void draw(Graphics& graphics) const = 0;
};

struct Rect
{
	Rect(int32_t _x = 0, int32_t _y = 0, int32_t _w = 0, int32_t _h = 0)
		: x(_x)
		, y(_y)
		, w(_w)
		, h(_h)
	{}
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
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

class Graphics {
public:
    Graphics(int32_t width, int32_t height, const std::string& title);
    ~Graphics();

	bool is_ok();
    
    void draw(const Drawable* object);
    void render_frame();
    void clear_frame();
    
	void draw_texture(const Texture& texture, const Rect& src, const Rect& dest, const Color& color = Color());
	void draw_square(const Rect& dest, const glm::vec4& color);
    
    int32_t get_width() const;
    int32_t get_height() const;
    
private:
	friend class Texture;
    
    SDL_Window*		window_;
	SDL_GLContext	gl_context_;
    int32_t w_;
    int32_t h_;

	bool ok_;
};

#endif
