#ifndef GRAPHICS_H
# define GRAPHICS_H

#include "vector.h"
#include "texture.h"

#include <cstdint>
#include <memory>
#include <string>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;

class Graphics;

class Drawable
{
public:
	virtual ~Drawable();

	virtual void draw(Graphics& graphics) const = 0;
};

struct Rect
{
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
};

struct Color
{
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
    
	void draw_texture(const Texture& texture, const Rect& src, const Rect& dest);
	void draw_square(const Rect& dest, const Color& color);
    
    int32_t get_width() const;
    int32_t get_height() const;
    
private:
	friend class Texture;
    
    SDL_Window*		window_;
	SDL_Renderer*	renderer_;
    int32_t w_;
    int32_t h_;

	bool ok_;
};

#endif
