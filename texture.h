#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <string>

typedef struct SDL_Texture SDL_Texture;

class Graphics;

class Texture
{
public:
	Texture();
	~Texture();

	void load(Graphics& graphics, const std::string& file_name);

private:
	friend class Graphics;

	SDL_Texture* texture_;
	int32_t w_;
	int32_t h_;
};

#endif
