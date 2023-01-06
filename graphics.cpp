#include "stdafx.h"

#include "graphics.h"
#include "log.h"

#include <iostream>

#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifdef _MSC_VER
#include <windows.h>
#endif // _MSC_VER


#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Graphics::Graphics(int32_t width, int32_t height, const std::string &title)
    : window_(nullptr)
	, gl_context_(nullptr)
    , w_(width)
    , h_(height)
	, ok_(true)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		ok_ = false;
        LOG_ERROR(std::string{"SDL video could not initialize! Error: "} + SDL_GetError());
        return;
	}

    /*int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init( imgFlags ) & imgFlags))
    {
		ok_ = false;
        LOG_ERROR(std::string{"SDL_image could not initialize! SDL_image Error: "} + IMG_GetError());
    }

    if(TTF_Init() == -1)
    {
		ok_ = false;
        LOG_ERROR(std::string{"SDL_ttf could not initialize! SDL_ttf Error: "} + TTF_GetError());
    }*/
    
    window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	gl_context_ = SDL_GL_CreateContext(window_);

	SDL_GL_SetSwapInterval(1);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GetWindowSize(window_, &w_, &h_);

	const glm::mat4 matrix = glm::ortho<float>(0, w_, h_, 0);
	glViewport(0, 0, w_, h_);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(matrix));
	glMatrixMode(GL_MODELVIEW);
}

Graphics::~Graphics()
{
	if (gl_context_) {
		SDL_GL_DeleteContext(gl_context_);
		gl_context_ = nullptr;
	}
	if (window_) {
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}

	//TTF_Quit();
	//IMG_Quit();
	SDL_Quit();
}

void Graphics::render_frame()
{
	SDL_GL_SwapWindow(window_);
}

void Graphics::clear_frame()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

bool Graphics::is_ok() { 
	return ok_;
}


void Graphics::draw(const Drawable* object)
{
	object->draw(*this);
}



Drawable::~Drawable() {
}


void Graphics::draw_texture(const Texture &texture, const Rect &src, const Rect &dest, const Color& color) {

}


int32_t Graphics::get_height() const {
    return h_;
}


int32_t Graphics::get_width() const { 
    return w_;
}

void ValidateOpenGLErrors()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::string message;
		switch (error)
		{
		case GL_INVALID_ENUM:
			message = "invalid enum passed to GL function (GL_INVALID_ENUM)";
			break;
		case GL_INVALID_VALUE:
			message = "invalid parameter passed to GL function (GL_INVALID_VALUE)";
			break;
		case GL_INVALID_OPERATION:
			message = "cannot execute some of GL functions in current state (GL_INVALID_OPERATION)";
			break;
		case GL_STACK_OVERFLOW:
			message = "matrix stack overflow occured inside GL (GL_STACK_OVERFLOW)";
			break;
		case GL_STACK_UNDERFLOW:
			message = "matrix stack underflow occured inside GL (GL_STACK_UNDERFLOW)";
			break;
		case GL_OUT_OF_MEMORY:
			message = "no enough memory to execute GL function (GL_OUT_OF_MEMORY)";
			break;
		default:
			message = "error in some GL extension (framebuffers, shaders, etc)";
			break;
		}
		std::cerr << "OpenGL error: " << message << std::endl;
		std::abort();
	}
}


void Graphics::draw_square(const Rect &dest, const glm::vec4 &color) {
	glColor3f(color.r, color.g, color.b);
	/*glBegin(GL_LINE_LOOP);
	glVertex2f(dest.x, dest.y);
	glVertex2f(dest.x, dest.y + dest.h);
	glVertex2f(dest.x + dest.w, dest.y + dest.h);
	glVertex2f(dest.x + dest.w, dest.y);
	glEnd();
	ValidateOpenGLErrors();*/

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(dest.x, dest.y);
	glVertex2f(dest.x, dest.y + dest.h);
	glVertex2f(dest.x + dest.w, dest.y + dest.h);
	glVertex2f(dest.x + dest.w, dest.y);
	glEnd();
	ValidateOpenGLErrors();
}
