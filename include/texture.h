#ifndef TEXTURE_H
#define TEXTURE_H

#include "Export.h"
#include "Common.hpp"

#include <stdint.h>
#include <string>
#include <memory>

typedef struct SDL_Texture SDL_Texture;

class Graphics;

class Texture;
using TexturePtr = Ptr<Texture>;

class TWODCPP_EXPORT Texture
{
public:
	Texture();
	~Texture();

	Texture(const Texture& texture);

	bool load(Graphics& graphics, const std::string& file_name);
    
    int32_t get_w() const;
    int32_t get_h() const;

private:
	friend class Graphics;
	std::shared_ptr<SDL_Texture> texture_;
	int32_t w_;
	int32_t h_;
};

#endif
