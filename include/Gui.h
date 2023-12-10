#pragma once

#include "Export.h"

#include <list>

class Game;
class Widget;

class TWODCPP_EXPORT Gui
{
public:
    void update(Game& game);
    void render(Game& game);

    void add(Widget* w);
    void remove(Widget* w);

private:
    std::list<Widget*> widgets_;
};