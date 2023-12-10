#include "Gui.h"

#include "Widgets.h"

void Gui::update(Game& game)
{
    for (Widget* w : widgets_)
    {
        w->update(game);
    }
}
void Gui::render(Game & game)
{
    for (Widget* w : widgets_)
    {
        w->render(game);
    }
}

void Gui::add(Widget* w)
{
    widgets_.push_back(w);
}

void Gui::remove(Widget* w)
{
    widgets_.erase(std::remove(widgets_.begin(), widgets_.end(), w), widgets_.end());
}