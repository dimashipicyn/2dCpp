//
//  Sprite_atlas.cpp
//  2dCpp
//
//  Created by Dmitry Shipicyn on 14.09.2022.
//

#include "Sprite_atlas.hpp"
#include "game.h"
#include "graphics.h"
#include "log.h"

#include "json/json.h"
#include <fstream>
#include <iostream>

Sprite_atlas::Sprite_atlas()
    : sprites_()
{
}


Sprite_atlas::~Sprite_atlas() {
}


bool Sprite_atlas::load(Game &game, const std::string file_name) {
    try {
        std::ifstream ifs(file_name);
        if (ifs.is_open()) {
            Json::Value root;
            Json::CharReaderBuilder builder;
            builder["collectComments"] = true;
            JSONCPP_STRING errs;
            if (!parseFromStream(builder, ifs, &root, &errs)) {
                LOG_ERROR("Sprite atlas json parse error: " + errs);
                return false;
            }
            load_sprites(game, root);
            return true;
        }
        LOG_INFO("Sprite atlas loading fail. Dont open file: " + file_name);
    }
    catch (const std::exception& e) {
        LOG_ERROR(std::string{"Sprite atlas loading fail. Error: "} + e.what());
    }
    
    return false;
}


Sprite Sprite_atlas::get(const std::string name) {
    auto it = sprites_.find(name);
    if (it != sprites_.end()) {
        return it->second;
    }
    LOG_WARNING("Sprite atlas not find sprite: " + name);
    return Sprite{};
}

void Sprite_atlas::load_sprites(Game& game, const Json::Value& value) {
    const Json::Value& meta = value.get("meta", Json::Value());
    std::string texture_name = meta.get("image", Json::String()).asString();
    
    Texture texture;
    if (!texture.load(game.get_graphics(), texture_name)) {
        return;
    }
    
    const Json::Value& frame_objs = value.get("frames", Json::Value());
    const auto& names = frame_objs.getMemberNames();
    for (const auto& name : names) {
        const auto& frame_obj = frame_objs.get(name, Json::Value());
        const Json::Value& frame = frame_obj.get("frame", Json::Value());
        int x = frame.get("x", 0).asInt();
        int y = frame.get("y", 0).asInt();
        int w = frame.get("w", 0).asInt();
        int h = frame.get("h", 0).asInt();
        
        Sprite sprite{texture, Rect{x,y,w,h}, Rect{0,0,w,h}};
        sprites_.insert({name, std::move(sprite)});
    }
}

