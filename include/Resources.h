#pragma once

#include "Export.h"

#include "Font.h"
#include "game.h"
#include "Common.hpp"

#include <string>
#include <array>
#include <unordered_map>

class TWODCPP_EXPORT Resources
{
    const char* prefix = "../assets/";

public:
    Resources(Game& game)
        : game_ { game }
    {
    }
	
    template<class T, class... Args>
    Ptr<T> get(const std::string& name, Args&&... args)
    {
        static std::unordered_map<std::string, Ptr<T>> resources;

        if (resources.count(name))
        {
            return resources[name];
        }

        Ptr<T> r = CreatePtr<T>();
        if (r->load(game_, prefix + name, std::forward<Args>(args)...))
        {
            resources.insert(std::make_pair(name, r));
            return r;
        }
        assert(false);
        return nullptr;
    }

    /*template <>
    FontPtr get<Font>(const std::string& name, int size)
    {
        if (fonts_.count(name))
        {
            return fonts_[name];
        }

        FontPtr font = CreatePtr<Font>();
        if (font->load(game_->get_graphics(), prefix + name, size))
        {
            fonts_.insert(std::make_pair(name, font));
            return font;
        }
        assert(false);
        return nullptr;
    }*/

private:
    Game& game_ ;
};