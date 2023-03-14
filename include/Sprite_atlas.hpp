//
//  Sprite_atlas.hpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 14.09.2022.
//

#ifndef Sprite_atlas_hpp
#define Sprite_atlas_hpp

#include "Sprite.h"

#include <map>
#include <string>

namespace Json {
    class Value;
}

class Sprite_atlas
{
public:
    Sprite_atlas();
    ~Sprite_atlas();
    
    bool load(Game& game, const std::string file_name);
    
    Sprite get(const std::string name);
    
private:
    void load_sprites(Game& game, const Json::Value& value);
    
    std::map<std::string, Sprite> sprites_;
};

#endif /* Sprite_atlas_hpp */
