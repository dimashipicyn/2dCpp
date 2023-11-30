#include "Widgets.h"

#include "graphics.h"
#include "input.h"
#include "game.h"
#include "Provider.h"

#include <cassert>
#include <glm/ext/vector_float2.hpp>

Widget::Widget(Widget* parent)
    : parent_ { parent }
{
    if (parent_)
    {
        parent_->childs_.push_front(this);
    }
}

Widget::~Widget()
{
    if (parent_)
    {
        for (Widget* w : childs_)
        {
            delete w;
        }
        parent_->childs_.erase(std::remove(parent_->childs_.begin(), parent_->childs_.end(), this), parent_->childs_.end());
    }
}

void Widget::update(Game& game)
{
    if (!enabled())
    {
        return;
    }

    glm::vec2 mouse = game.get_input().get_mouse_position();
    bool leftPresed = game.get_input().get_button(Input::MouseLeft);
    bool rightPresed = game.get_input().get_button(Input::MouseRight);

    int x = parent_ ? parent_->pos().x + pos().x : pos().x;
    int y = parent_ ? parent_->pos().y + pos().y : pos().y;
    Rect dest(x, y, size().w, size().h);
    if (dest.contains(mouse.x, mouse.y))
    {
        inFocus_ = true;
        if (!onEnter_)
        {
            invoke(WidgetSignal::Enter);
            onEnter_ = true;
        }
        if (leftPresed)
        {
            invoke(WidgetSignal::LeftClick);
        }
        if (rightPresed)
        {
            invoke(WidgetSignal::RightClick);
        }
    }
    else
    {
        inFocus_ = false;
        if (onEnter_)
        {
            invoke(WidgetSignal::Leave);
            onEnter_ = false;
        }
    }

    for (Widget* c : childs_)
    {
        c->update(game);
    }
}

void Widget::render(Game& game)
{
    if (!enabled())
    {
        return;
    }

    for (Widget* c : childs_)
    {
        c->render(game);
    }
}

Label::Label(const std::string& text, Widget* parent)
    : Widget(parent)
    , text_(text)
{
    font_ = Provider::get().get<Resources>().get<Font>("gui.ttf", 14);
    Font::Size s = font_->get_str_size(text_.c_str());
    setSize(Size(s.w, s.h));
}

Label::~Label()
{

}

void Label::update(Game& game)
{
    Widget::update(game);
}

void Label::render(Game& game)
{
    if (!enabled())
    {
        return;
    }

    int x = parent_ ? parent_->pos().x + pos().x : pos().x;
    int y = parent_ ? parent_->pos().y + pos().y : pos().y;
    game.get_graphics().draw_str(*font_,
        x,
        y,
        text_.c_str(),
        color_);
    Widget::render(game);
}

void Label::set_color(const Color& color)
{
    color_ = color;
}

void Label::set_text(const std::string& text)
{
    text_ = text;
    Font::Size s = font_->get_str_size(text.c_str());
    setSize(Size(s.w, s.h));
}

Button::Button(const std::string& text, Widget* parent)
    : Label(text, parent)
{
}

Button::~Button()
{

}

void Button::update(Game &game)
{
    Label::update(game);
}

void Button::render(Game &game)
{
    Color save = color_;
    color_ = inFocus() ? color_green : color_;
    Label::render(game);
    color_ = save;
}

Select::Select(Widget* parent)
    : Widget(parent)
    , left("<", this)
    , right(">", this)
{
    right.setPos(Point(left.pos().x + left.size().w, 0));

    left.on(WidgetSignal::LeftClick, [this]()
        {
            if (options.empty())
                return;
            options[current_option]->setEnabled(false);
            current_option = std::max(current_option - 1, 0);
            options[current_option]->setEnabled(true);
        });
    right.on(WidgetSignal::LeftClick, [this]()
        {
            if (options.empty())
                return;
            options[current_option]->setEnabled(false);
            current_option = std::min(current_option + 1, (int)options.size() - 1);
            options[current_option]->setEnabled(true);
        });
}

Select::~Select()
{

}

void Select::update(Game& game)
{
    if (!enabled())
    {
        return;
    }

    int rightX = left.pos().x + left.size().w + spacing().w;
    if (!options.empty())
    {
        auto& opt = options[current_option];
        opt->setPos(Point(rightX, opt->pos().y));
        rightX += opt->size().w + spacing().w;
    }
    right.setPos(Point(rightX, right.pos().y));

    Widget::update(game);
}

void Select::render(Game& game)
{
    Widget::render(game);
}

void Select::add_option(const std::string& opt)
{
    options.emplace_back(CreatePtr<Label>(opt, this));
    options.back()->setEnabled(false);
    options[current_option]->setEnabled(true);
}

Grid::Grid(int cells_w, int cells_h, Widget* parent)
    : Widget(parent)
    , cells_w(cells_w)
    , cells_h(cells_h)

{

}

Grid::~Grid()
{

}

void Grid::update(Game& game)
{
    Widget::update(game);
}

void Grid::render(Game& game)
{
    Widget::render(game);
}

void Grid::set(int row, int col, Widget* widget)
{
    //childs_[row * cells_w + col] = std::move(widget);

    //for (int i = 0; i < cells_h; i++) {
    //    for (int j = 0; j < cells_w; j++) {
    //        auto& c = childs[i * cells_w + j];
    //        if (!c)
    //            break;
    //        
    //        c->set_x(x + j * cell_size_w);// + c->get_x());
    //        c->set_y(y + i * cell_size_h);// + c->get_y());
    //    }
    //}
}

Slider::Slider(float step, float value, Widget* parent)
    : Widget(parent)
    , left_("<", this)
    , right_(">", this)
    , step_(step)
{
    int w = left_.size().w + right_.size().w;
    int h = std::max(left_.size().h, right_.size().h);
    setSize(Size(w, h));
    setSpacing(Size(1, 1));

    left_.on(WidgetSignal::LeftClick, [this]()
        {
        value_ = std::max(value_ - this->step_, 0.0f);
    });
    right_.on(WidgetSignal::LeftClick, [this]()
        {
        value_ = std::min(value_ + this->step_, 1.0f);
    });
}

Slider::~Slider()
{

}

void Slider::update(Game& game)
{
    if (!enabled())
    {
        return;
    }

    int buttonsW = left_.size().w + right_.size().w;
    int buttonsH = std::max(left_.size().h, right_.size().h);
    if (size().w == 0)
    {
        setSize(Size(buttonsW + 2, size().h));
    }
    if (size().h == 0)
    {
        setSize(Size(size().w, buttonsH * 2));
    }

    int sliderOutlineX = left_.pos().x + left_.size().w + spacing().w;
    sliderOutline_ = Rect(sliderOutlineX, pos().y, size().w - buttonsW, size().h);

    int rightX = sliderOutline_.x + sliderOutline_.w + spacing().w;
    right_.setPos(Point(rightX, right_.pos().y));

    Widget::update(game);
}

void Slider::render(Game& game)
{
    if (!enabled())
    {
        return;
    }

    int w = (sliderOutline_.w - 2) * value();
    Rect slider(sliderOutline_.x + 1, sliderOutline_.y + 1, w, sliderOutline_.h - 2);

    game.get_graphics().draw_outline_rect(sliderOutline_, Color());
    game.get_graphics().draw_rect(slider, color_green);

    Widget::render(game);
}
