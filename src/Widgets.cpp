#include "Widgets.h"

#include "graphics.h"
#include "input.h"
#include "game.h"
#include "Provider.h"
#include "Resources.h"

#include <cassert>
#include <glm/ext/vector_float2.hpp>

Widget::Widget(Widget* parent)
    : parent_ { parent }
{
    setParent(parent);
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

    for (Widget* c : childs_)
    {
        c->update(game);
    }

    glm::vec2 mouse = game.get_input().get_mouse_position();
    bool leftPresed = game.get_input().get_button(Input::MouseLeft);
    bool rightPresed = game.get_input().get_button(Input::MouseRight);

    Point wPos = worldPos();
    Rect dest(wPos.x, wPos.y, size().w, size().h);
    if (dest.contains(mouse.x, mouse.y))
    {
        inFocus_ = true;
        if (!onEnter_)
        {
            invoke(Enter);
            onEnter_ = true;
        }
        if (leftPresed)
        {
            invoke(LeftClick);
        }
        if (rightPresed)
        {
            invoke(RightClick);
        }
    }
    else
    {
        inFocus_ = false;
        if (onEnter_)
        {
            invoke(Leave);
            onEnter_ = false;
        }
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

void Layout::add(Widget* w)
{
    w->setParent(this);
    w->on(Resize, this, &Layout::recalc);
    recalc();
}

HLayout::HLayout(Widget* parent)
    : Layout(parent)
{
}

void HLayout::recalc()
{
    int newW = 0;
    int newH = 0;
    for (Widget* w : childs())
    {
        newH = std::max(newH, w->size().h);
        w->move(Point(newW, w->pos().y));
        newW += w->size().w + margin();
    }
    if (childs().size())
    {
        newW -= margin();
    }
    size_ = Size(newW, newH);
}

VLayout::VLayout(Widget* parent)
    : Layout(parent)
{
}

void VLayout::recalc()
{
    int newW = 0;
    int newH = 0;
    for (Widget* w : childs())
    {
        newW = std::max(newW, w->size().w);
        w->move(Point(w->pos().x, newH));
        newH += w->size().h + margin();
    }
    if (childs().size())
    {
        newH -= margin();
    }
    size_ = Size(newW, newH);
}

Label::Label(const std::string& text, Widget* parent)
    : Widget(parent)
    , text_(text)
{
    font_ = Provider::get().get<Resources>().get<Font>("EightBits.ttf", 35);
    Font::Size s = font_->get_str_size(text_.c_str());
    resize(Size(s.w, s.h));
}

void Label::render(Game& game)
{
    if (!enabled())
    {
        return;
    }

    Point pos = worldPos();
    game.get_graphics().draw_str(*font_,
        pos.x,
        pos.y,
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
    resize(Size(s.w, s.h));
}

Button::Button(const std::string& text, Widget* parent)
    : Widget(parent)
    , text_(text, this)
    , color_()
    , borderColor_(color_white)
{
    text_.move(Point(3, 3));
    resize(Size(text_.size().w + 6, text_.size().h + 6));
}

void Button::render(Game &game)
{
    Rect border(worldPos().x, worldPos().y, size().w, size().h);
    game.get_graphics().draw_outline_rect(border, borderColor_);

    Widget::render(game);
}

Select::Select(Widget* parent)
    : Widget(parent)
    , hlayout(this)
    , option("default")
{
    hlayout.setMargin(10);
    hlayout.on(Resize, [this](const Widget* w)
        { resize(w->size()); });

    auto left = new Button("<");
    auto right = new Button(">");
    hlayout.add(left);
    hlayout.add(&option);
    hlayout.add(right);

    left->on(WidgetSignal::LeftClick, [this]()
        {
            if (options.empty())
                return;
            current_option = std::max(current_option - 1, 0);
            option.set_text(options[current_option]);
        });
    right->on(WidgetSignal::LeftClick, [this]()
        {
            if (options.empty())
                return;
            current_option = std::min(current_option + 1, (int)options.size() - 1);
            option.set_text(options[current_option]);
        });
}

void Select::add_option(const std::string& opt)
{
    options.emplace_back(opt);
    option.set_text(options[current_option]);
}

Slider::Slider(float step, float value, Widget* parent)
    : Widget(parent)
    , left_("<", this)
    , right_(">", this)
    , step_(step)
    , value_(value)
{
    on(Resize, [this]
        {
            int x = left_.size().w + 10;
            int w = size().w - left_.size().w - right_.size().w - 10;
            outline_ = Rect(x, pos().y, w, size().h);
            right_.move(Point(outline_.x + outline_.w + 10, 0));
        });

    resize(Size(left_.size().w + 10 + right_.size().w + 10, left_.size().h));
    
    left_.on(WidgetSignal::LeftClick, this, [this]()
        { value_ = std::max(value_ - this->step_, 0.0f); });
    right_.on(WidgetSignal::LeftClick, [this]()
        { value_ = std::min(value_ + this->step_, 1.0f); });
}

void Slider::render(Game& game)
{
    Rect outline(worldPos().x + outline_.x, worldPos().y + outline_.y, outline_.w, outline_.h);
    game.get_graphics().draw_outline_rect(outline, color_white);

    int w = (outline.w - 2) * value();
    if (w > 0)
    {
        Rect slider(outline.x + 1, outline.y + 1, w, outline.h - 2);
        game.get_graphics().draw_rect(slider, color_green);
    }

    Widget::render(game);
}

