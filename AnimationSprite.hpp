//
//  AnimationSprite.hpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 16.09.2022.
//

#ifndef AnimationSprite_hpp
#define AnimationSprite_hpp

#include "Sprite.h"

#include <vector>

class AnimationSprite : public Sprite
{
public:
    AnimationSprite();
    AnimationSprite(const Texture& texture, const std::vector<Rect>& src_list, const Rect& dest);
    ~AnimationSprite();
    
    bool load(Graphics& graphics, const std::string& path_to_texture, int frames_in_animation);
    
    void play();
    void stop();
    
    void set_duration_in_ms(int ms);
    
    void draw(Graphics& graphics);
private:
    std::vector<Rect> animation_list_;
    int frame_index_;
    int duration_in_ms_;
    bool playing_;
};

#endif /* AnimationSprite_hpp */
