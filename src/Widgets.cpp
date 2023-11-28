#include "Widgets.h"

#include "graphics.h"
#include "input.h"
#include "game.h"

#include <cassert>
#include <glm/ext/vector_float2.hpp>

Widget::Widget(const std::string& name, int x, int y)
    : name(name)
    , x(x)
    , y(y)
{

}

Widget::~Widget()
{

}

int Widget::get_x()
{
    return x;
}

int Widget::get_y()
{
    return y;
}

int Widget::get_w()
{
    return w;
}

int Widget::get_h()
{
    return h;
}

WidgetLabel::WidgetLabel(FontPtr font, int x, int y, const std::string &name, const std::string &label)
    : Widget(name, x, y)
    , font(font)
    , label(label)
{
    Font::Size s = font->get_str_size(label.c_str());
    w = s.w;
    h = s.h;
}

WidgetLabel::~WidgetLabel()
{

}

void WidgetLabel::update(Game &)
{
    
}

void WidgetLabel::render(Game &game)
{
    game.get_graphics().draw_str(*font, x, y, label.c_str(), color);
}

void WidgetLabel::set_color(const Color& color)
{
    this->color = color;
}

void WidgetLabel::set_label(const std::string& label)
{
    this->label = label;
    Font::Size s = font->get_str_size(label.c_str());
    w = s.w;
    h = s.h;
}

WidgetButton::WidgetButton(FontPtr font, int x, int y, const std::string &name, const std::string &text)
    : Widget(name, x, y)
    , font(font)
    , text(text)
{
    Font::Size s = font->get_str_size(text.c_str());
    w = s.w;
    h = s.h;
}

WidgetButton::~WidgetButton()
{

}

void WidgetButton::update(Game &game)
{
    glm::vec2 mouse = game.get_input().get_mouse_position();
    bool is_pressed = game.get_input().get_button(Input::MouseLeft);

    Rect dest(x, y, w, h);
    if (dest.contains(mouse.x, mouse.y))
    {
        in_focus = true;
        if (is_pressed && cb)
        {
            cb();
        }
    }
    else {
        in_focus = false;
    }
}

void WidgetButton::render(Game &game)
{
    Color c = in_focus ? color_green : color;
    game.get_graphics().draw_str(*font, x, y, text.c_str(), c);
}

void WidgetButton::set_color(const Color& color)
{
    this->color = color;
}

void WidgetButton::set_text(const std::string& text)
{
    this->text = text;
}

void WidgetButton::on_click(on_click_callback cb)
{
    this->cb = cb;
}

WidgetSelect::WidgetSelect(FontPtr font, int x, int y, const std::string& name)
    : Widget(name, x, y)
    , left(font, x, y, "", "< ")
    , right(font, x + font->get_str_size("< ").w, y, "", " >")
    , label(font, x, y, "", "")
{
    left.on_click([this](){
        current_option = std::max(current_option - 1, 0);
    });
    right.on_click([this](){
        if (options.empty())
            return;
        current_option = std::min(current_option + 1, (int)options.size() - 1);
    });
}

WidgetSelect::~WidgetSelect()
{

}

void WidgetSelect::update(Game& game)
{
    left.update(game);
    right.update(game);
    label.update(game);

    if (options.empty())
    {
        return;
    }

    label.set_label(options[current_option]);
}

void WidgetSelect::render(Game& game)
{
    left.set_x(x);
    label.set_x(left.get_x() + left.get_w());
    right.set_x(label.get_x() + label.get_w());

    left.set_y(y);
    label.set_y(y);
    right.set_y(y);

    left.render(game);
    label.render(game);
    right.render(game);
}

void WidgetSelect::set_color(const Color& color)
{
    left.set_color(color);
    right.set_color(color);
    label.set_color(color);
}

void WidgetSelect::add_option(const std::string& opt)
{
    options.emplace_back(opt);   
}

WidgetGrid::WidgetGrid(const std::string& name, int x, int y, int cells_w, int cells_h, int cell_size_w, int cell_size_h)
    : Widget(name, x, y)
    , cells_w(cells_w)
    , cells_h(cells_h)
    , cell_size_w(cell_size_w)
    , cell_size_h(cell_size_h)
{
    childs.resize(cells_w * cells_h);
    w = cells_w * cell_size_w;
    h = cells_h * cell_size_h;
}

WidgetGrid::~WidgetGrid()
{

}

void WidgetGrid::update(Game& game)
{
    for (WidgetPtr& w : childs) {
        if (w)
            w->update(game);
    }
}

void WidgetGrid::render(Game& game)
{
    for (WidgetPtr& w : childs) {
        if (w)
            w->render(game);
    }
}

void WidgetGrid::set(int row, int col, WidgetPtr widget)
{
    childs[row * cells_w + col] = std::move(widget);

    for (int i = 0; i < cells_h; i++) {
        for (int j = 0; j < cells_w; j++) {
            auto& c = childs[i * cells_w + j];
            if (!c)
                break;
            
            c->set_x(x + j * cell_size_w);
            c->set_y(y + i * cell_size_h);
        }
    }
}

WidgetSlider::WidgetSlider(FontPtr font, int x, int y, int size, int step, const std::string& name)
    : Widget(name, x, y)
    , left(font, x, y, "", "< ")
    , right(font, x + font->get_str_size("< ").w, y, "", " >")
    , step(step)
    , size(size)
{
    left.on_click([this](){
        value = std::max(value - this->step, 0);
    });
    right.on_click([this](){
        value = std::min(value + this->step, this->size - 2);
    });
}

WidgetSlider::~WidgetSlider()
{

}

void WidgetSlider::update(Game& game)
{
    left.update(game);
    right.update(game);
}

void WidgetSlider::render(Game& game)
{
    left.set_x(x);
    int slider_y = y + ceil(left.get_h() / 2.0f) - 10;
    Rect slider_outline(left.get_x() + left.get_w(), slider_y, size, 20);
    Rect slider(left.get_x() + left.get_w() + 1, slider_y + 1,value, 18);
    right.set_x(slider_outline.x + slider_outline.w);

    left.set_y(y);
    right.set_y(y);

    left.render(game);
    game.get_graphics().draw_outline_rect(slider_outline, Color());
    game.get_graphics().draw_rect(slider, color_green);
    right.render(game);
}