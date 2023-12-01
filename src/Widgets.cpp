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
        parent_->childs_.push_back(this);
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

HLayout::HLayout(Widget* parent)
    : Widget(parent)
{
}

void HLayout::add(Widget* w)
{
    w->setParent(this);
}

void HLayout::update(Game& game)
{
    Widget::update(game);

    int maxH = 0;
    int x = spacing().w;
    for (Widget* w : childs_)
    {
        maxH = std::max(maxH, w->size().h);
        w->setPos(Point(x, w->pos().y));
        x += w->size().w + spacing().w;
    }
    setSize(Size(x, maxH));
}

VLayout::VLayout(Widget* parent)
    : Widget(parent)
{
}

void VLayout::add(Widget* w)
{
    w->setParent(this);
}

void VLayout::update(Game& game)
{
    Widget::update(game);

    int maxW = 0;
    int y = spacing().h;
    for (Widget* w : childs_)
    {
        maxW = std::max(maxW, w->size().w);
        w->setPos(Point(w->pos().x, y));
        y += w->size().h + spacing().h;
    }
    setSize(Size(maxW, y));
}

Label::Label(const std::string& text, Widget* parent)
    : Widget(parent)
    , text_(text)
{
    font_ = Provider::get().get<Resources>().get<Font>("EightBits.ttf", 35);
    Font::Size s = font_->get_str_size(text_.c_str());
    setSize(Size(s.w, s.h));
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
    setSize(Size(s.w, s.h));
}

Button::Button(const std::string& text, Widget* parent)
    : Label(text, parent)
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
    , hlayout(this)
    , option("default")
{
    auto left = new Button("<", &hlayout);
    option.setParent(&hlayout);
    auto right = new Button(">", &hlayout);

    left->on(WidgetSignal::LeftClick, [this]()
        {
            if (options.empty())
                return;
            current_option = std::max(current_option - 1, 0);
        });
    right->on(WidgetSignal::LeftClick, [this]()
        {
            if (options.empty())
                return;
            current_option = std::min(current_option + 1, (int)options.size() - 1);
        });
}

void Select::update(Game& game)
{
    if (!enabled())
    {
        return;
    }

    Widget::update(game);
    option.set_text(options[current_option]);
    setSize(hlayout.size());
}

void Select::render(Game& game)
{
    Widget::render(game);
}

void Select::add_option(const std::string& opt)
{
    options.emplace_back(opt);
}

Slider::Slider(float step, float value, Widget* parent)
    : Widget(parent)
    , layout_(this)
    , left_("<")
    , right_(">")
    , step_(step)
    , value_(value)
{
    left_.setParent(&layout_);
    outline_.setParent(&layout_);
    rect_.setPos(Point(1, 1));
    rect_.setParent(&outline_);
    right_.setParent(&layout_);

    left_.on(WidgetSignal::LeftClick, this, [this]()
        { value_ = std::max(value_ - this->step_, 0.0f); });
    right_.on(WidgetSignal::LeftClick, [this]()
        { value_ = std::min(value_ + this->step_, 1.0f); });

    calculate();
}

void Slider::render(Game& game)
{
    if (!enabled())
    {
        return;
    }

    Widget::render(game);
}

void Slider::calculate()
{
    int buttonsW = left_.size().w + right_.size().w;
    int buttonsH = std::max(left_.size().h, right_.size().h);
    if (size().w == 0)
    {
        size_ = Size(buttonsW, size().h);
    }
    if (size().h == 0)
    {
        size_ = Size(size().w, buttonsH);
    }
    int w = std::max(size().w - buttonsW - 1, 3);
    int h = std::max(size().h, buttonsH);
    outline_.setSize(Size(w, h));
    rect_.setSize(Size(outline_.size().w - 2, outline_.size().h - 2));
}

void Rectangle::render(Game& game)
{
    auto wPos = worldPos();
    Rect rect(wPos.x, wPos.y, size().w, size().h);
    if (fill_)
    {
        game.get_graphics().draw_rect(rect, color_);
    }
    else
    {
        game.get_graphics().draw_outline_rect(rect, color_);
    }
}
