#include "graphics.h"
#include "log.h"
#include "Sprite.h"
#include "texture.h"
#include "Font.h"

#include <SDL_rect.h>
#include <SDL_render.h>
#include <math.h>
#include <SDL.h>

Graphics::Graphics(int32_t width, int32_t height, const std::string &title)
    : window_(nullptr)
    , renderer_(nullptr)
    , w_(width)
    , h_(height)
	, background_(Color(0,0,0,0))
	, ok_(true)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		ok_ = false;
        LOG_ERROR(std::string{"SDL video could not initialize! Error: "} + SDL_GetError());
        return;
	}
    
    window_ = SDL_CreateWindow(title.c_str(),width, height, 0);
    renderer_ = SDL_CreateRenderer(window_, nullptr, 0);

	if (!window_ || !renderer_) {
		ok_ = false;
        LOG_ERROR(std::string{"SDL could not initialize! Error: "} + SDL_GetError());
	}

	SDL_GetWindowSize(window_, &w_, &h_);
	SDL_SetRenderDrawColor(renderer_, 0x0, 0x0, 0x0, 0x0);
}

Graphics::~Graphics()
{
	if (renderer_) {
		SDL_DestroyRenderer(renderer_);
		renderer_ = nullptr;
	}
	if (window_) {
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}

	SDL_Quit();
}

void Graphics::render_frame()
{
	SDL_RenderPresent(renderer_);
}

void Graphics::clear_frame()
{
	SDL_RenderClear(renderer_);
}

bool Graphics::is_ok() { 
	return ok_;
}

void Graphics::set_background_color(const Color& color)
{
	background_ = color;
}

int32_t Graphics::get_width() const { 
    return w_;
}

int32_t Graphics::get_height() const {
    return h_;
}

void Graphics::draw_texture(const Texture &texture, const Rect &src, const Rect &dest, const Color& color) {
	SDL_FRect s{(float)src.x, (float)src.y, (float)src.w, (float)src.h};
	SDL_FRect d{(float)dest.x, (float)dest.y, (float)dest.w, (float)dest.h};

	Color saved_color;
	SDL_GetTextureColorMod(texture.texture_.get(), &saved_color.r, &saved_color.g, &saved_color.b);

	SDL_SetTextureColorMod(texture.texture_.get(), color.r, color.g, color.b);
	SDL_RenderTexture(renderer_, texture.texture_.get(), &s, &d);
	SDL_SetTextureColorMod(texture.texture_.get(), saved_color.r, saved_color.g, saved_color.b);
}

void Graphics::draw_rect(const Rect &dest, const Color &color) { 
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_FRect dst{(float)dest.x, (float)dest.y, (float)dest.w, (float)dest.h};
	SDL_RenderFillRect(renderer_, &dst);
	SDL_SetRenderDrawColor(renderer_, background_.r, background_.g, background_.b, background_.a);
}

void Graphics::draw_outline_rect(const Rect& dest, const Color& color)
{
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_FRect dst{(float)dest.x, (float)dest.y, (float)dest.w, (float)dest.h};
	SDL_RenderRect(renderer_, &dst);
	SDL_SetRenderDrawColor(renderer_, background_.r, background_.g, background_.b, background_.a);
}

void Graphics::draw_pixel(int x, int y, const Color& color)
{
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderPoint(renderer_, x, y);
	SDL_SetRenderDrawColor(renderer_, background_.r, background_.g, background_.b, background_.a);
}

void Graphics::draw_line(int x1, int y1, int x2, int y2, const Color& color)
{
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderLine(renderer_, x1, y1, x2, y2);
	SDL_SetRenderDrawColor(renderer_, background_.r, background_.g, background_.b, background_.a);
}

void Graphics::draw_char(const Font& font, int x, int y, char c, const Color& color)
{
	Font::Glyph gl = font.get_glyph(c);
	SDL_FRect src = {(float)gl.src.x, (float)gl.src.y, (float)gl.src.w, (float)gl.src.h};
	float xPos = x + gl.bearingX;
    float yPos = y - gl.bearingY + font.get_size().h;
	SDL_FRect dest = {xPos, yPos, src.w, src.h};

	Color saved_color;
	SDL_GetTextureColorMod(gl.texture, &saved_color.r, &saved_color.g, &saved_color.b);
	
	SDL_SetTextureColorMod(gl.texture, color.r, color.g, color.b);
	SDL_RenderTexture(renderer_, gl.texture, &src, &dest);
	SDL_SetTextureColorMod(gl.texture, saved_color.r, saved_color.g, saved_color.b);
}

void Graphics::draw_str(const Font& font, int x, int y, const char* str, const Color& color)
{
	for (const char* c = str; *c != '\0'; c++)
	{
		draw_char(font, x, y, *c, color);
		Font::Glyph gl = font.get_glyph(*c);
		x+= gl.advanceX;
	}
}

void Graphics::debugRenderFont(Font& font)
{
    SDL_RenderTexture(renderer_, font.texture, NULL, NULL);
}
