//
//  Sprite.h
//  2dCpp
//
//  Created by Dmitry Shipicyn on 11.09.2022.
//

#ifndef Sprite_h
#define Sprite_h

#include "entity.h"
#include "texture.h"
#include "graphics.h"

class Sprite : public Entity
{
public:
    Sprite();
    ~Sprite() noexcept;
    
    void input(Game& game);
    void update(Game& game);
    void render(Game& game);
    
    bool load(Game& game, const std::string& file_name);
    
    void set_scale(float pt);
    void set_width(int pt);
    void set_height(int pt);
    
    float get_scale() const;
    int get_width() const;
    int get_height() const;
    
private:
    Texture texture_;
    Rect    src_;
    Rect    dest_;
    float   scale_;
};

#endif /* Sprite_h */
