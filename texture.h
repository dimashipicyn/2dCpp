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

	bool load(Graphics& graphics, const std::string& file_name);
    
    int32_t get_w() const;
    int32_t get_h() const;

private:
	friend class Graphics;

	SDL_Texture* texture_;
	int32_t w_;
	int32_t h_;
};

#endif
