//
//  Sprite.h
//  2dCpp
//
//  Created by Dmitry Shipicyn on 11.09.2022.
//

#ifndef Sprite_h
#define Sprite_h

#include "texture.h"
#include "graphics.h"
#include "Node.hpp"

class Sprite
{
public:
    Sprite();
	Sprite(const Sprite& s);
	Sprite(const Texture& texture);
    Sprite(const Texture& texture, const Rect& src, const Rect& dest);
    ~Sprite() noexcept;
    
    bool load(Graphics& graphics, const std::string& file_name);
    
    void set_scale(float pt);
    void set_position(int x, int y);
    void set_size(int w, int h);
	void set_color(const Color& color);
    
    float get_scale() const;
    
    int get_x() const;
    int get_y() const;
    int get_width() const;
    int get_height() const;

	bool is_intersect(const Sprite& other);
    
    void draw(Graphics& graphics);

	friend class Graphics;
    
protected:
    Texture texture_;
    Rect    src_;
    Rect    dest_;
    float   scale_;
	Color	color_;
};

#endif /* Sprite_h */
