#pragma once

#include "Common.hpp"
#include "Font.h"
#include "Signal.h"

#include <functional>
#include <string>
#include <vector>
#include <list>

class Game;
class Widget;
class Label;
class Select;
class Grid;
class Slider;
class Button;
class HLayout;
class VLayout;

using WidgetPtr = Ptr<Widget>;
using LabelPtr = Ptr<Label>;
using SelectPtr = Ptr<Select>;
using GridPtr = Ptr<Grid>;
using SliderPtr = Ptr<Slider>;
using ButtonPtr = Ptr<Button>;
using HLayoutPtr = Ptr<HLayout>;
using VLayoutPtr = Ptr<VLayout>;

enum class WidgetSignal
{
    LeftClick,
    RightClick,
    Changed,
    Enter,
    Leave,
    Move,
    Resize
};

enum class Align
{
    Top,
    Left,
    Right,
    Bottom,
    Center,
    HCenter,
    VCenter
};

class TWODCPP_EXPORT Widget : public Signal<Widget, WidgetSignal>
{
public:
    using enum WidgetSignal;
    using enum Align;

    Widget(Widget* parent = nullptr);
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    virtual ~Widget();

    Widget* parent() { return parent_; }
    void setParent(Widget* w)
    {
        if (parent_)
        {
            parent_->removeChild(this);
        }
        parent_ = w;
        if (parent_)
        {
            w->addChild(this);
        }
    }

    virtual void update(Game& game);
    virtual void render(Game& game);

    const Point worldPos() const
    {
        return parent_ ? parent_->worldPos() + pos() : pos();
    }

    const Point& pos() const { return pos_; }
    void move(const Point& pos)
    {
        pos_ = pos;
        invoke(Move);
    }

    const Size& size() const { return size_; }
    void resize(const Size& size)
    {
        size_ = size;
        invoke(Resize);
    }

    bool inFocus() const { return inFocus_; }
    bool enabled() const { return enabled_; }
    void setEnabled(bool enabled) { enabled_ = enabled; }

protected:
    void addChild(Widget* child)
    {
        childs_.push_back(child);
        invoke(Resize);
    }

    void removeChild(Widget* child)
    {
        childs_.erase(std::remove(childs_.begin(), childs_.end(), child), childs_.end());
    }

    std::list<Widget*>& childs() { return childs_; }

    Size size_;
private:
    Widget* parent_;
    std::list<Widget*> childs_;
    Point pos_;
    bool onEnter_ = false;
    bool inFocus_ = false;
    bool enabled_ = true;
};

class TWODCPP_EXPORT Layout : public Widget
{
public:
    Layout(Widget* parent = nullptr)
        : Widget(parent)
    {
        //on(Move, this, &Layout::recalc);
        //on(Resize, this, &Layout::recalc);
    }

    void add(Widget* w);

    int margin() const { return margin_; }
    void setMargin(int margin)
    {
        margin_ = margin;
        invoke(Resize);
    }

private:
    virtual void recalc() = 0;

private:
    int margin_ = 0;
};

class TWODCPP_EXPORT HLayout : public Layout
{
public:
    HLayout(Widget* parent = nullptr);

private:
    void recalc() override;
};

class TWODCPP_EXPORT VLayout : public Layout
{
public:
    VLayout(Widget* parent = nullptr);

private:
    void recalc() override;
};

class TWODCPP_EXPORT Label : public Widget
{
public:
    Label(const std::string& text, Widget* parent = 0);
    virtual void render(Game& game) override;

    void set_color(const Color& color);
    void set_text(const std::string& text);

protected:
    FontPtr font_;
    std::string text_;
    Color color_;
};

class TWODCPP_EXPORT Button : public Widget
{
public:
    Button(const std::string& text, Widget* parent = nullptr);
    virtual void render(Game& game) override;

    const Color& color() const { return color_; }
    void setColor(const Color& color) { color_ = color; }

    const Color& borderColor() const { return borderColor_; }
    void setborderColor(const Color& color) { borderColor_ = color; }

private:
    Label text_;
    Color color_;
    Color borderColor_;
};

class TWODCPP_EXPORT Select : public Widget
{
public:
    Select(Widget* parent = nullptr);
    void add_option(const std::string& opt);
    
    int currentIndex() const { return current_option; }
    std::string currentOption() const
    {
        assert(current_option > -1 && current_option < options.size());
        return options[current_option];
    }

private:
    HLayout hlayout;
    Label option;
    std::vector<std::string> options;
    int current_option = 0;
};

class TWODCPP_EXPORT Slider : public Widget
{
public:
    Slider(float step = 0.1f, float value = 0.0f, Widget* parent = nullptr);
    void render(Game& game) override;
    float step() const { return step_; }
    void setStep(float step) { step_ = std::min(step, 1.0f); }
    float value() const { return value_; }
    void setValue(float value) { value_ = std::min(value, 1.0f); }

private:
    Button left_;
    Button right_;
    Rect outline_;
    float step_ = 0;
    float value_ = 0;
};
