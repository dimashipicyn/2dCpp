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
    WidgetSignalCount
};

class TWODCPP_EXPORT Widget : public Signal<Widget, WidgetSignal, WidgetSignal::WidgetSignalCount>
{
public:
    Widget(Widget* parent = nullptr);
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    virtual ~Widget();

    Widget* parent() { return parent_; }
    void setParent(Widget* w)
    {
        w->childs_.push_back(this);
        parent_ = w;
    }

    virtual void update(Game& game);
    virtual void render(Game& game);

    const Point worldPos() const
    {
        return parent_ ? parent_->worldPos() + pos() : pos();
    }

    const Point& pos() const { return pos_; }
    void setPos(const Point& pos) { pos_ = pos; }

    const Size& size() const { return size_; }
    void setSize(const Size& size)
    {
        calculate();
        size_ = size;
    }

    const Size& spacing() const { return spacing_; }
    void setSpacing(const Size& spacing) { spacing_ = spacing; }

    bool inFocus() const { return inFocus_; }
    bool enabled() const { return enabled_; }
    void setEnabled(bool enabled) { enabled_ = enabled; }

private:
    virtual void calculate() { }

protected:
    Widget* parent_;
    std::list<Widget*> childs_;
    Point pos_;
    Size size_;
    Size spacing_;
    bool onEnter_ = false;
    bool inFocus_ = false;
    bool enabled_ = true;
};

class TWODCPP_EXPORT Rectangle : public Widget
{
public:
    Rectangle(Widget* widget = nullptr)
        : Widget(widget)
    {
    }
    virtual void render(Game& game) override;

    bool fill() const { return fill_; }
    void setFill(bool fill) { fill_ = fill; }

    const Color& color() const { return color_; }
    void setColor(const Color& color) { color_ = color; }

private:
    Color color_;
    bool fill_ = false;
};

class TWODCPP_EXPORT HLayout : public Widget
{
public:
    HLayout(Widget* parent = nullptr);
    void add(Widget* w);
    virtual void update(Game& game) override;
};

class TWODCPP_EXPORT VLayout : public Widget
{
public:
    VLayout(Widget* parent = nullptr);
    void add(Widget* w);
    virtual void update(Game& game) override;
};

class TWODCPP_EXPORT Label : public Widget
{
public:
    Label(const std::string& text, Widget* parent = 0);

    virtual void update(Game& game) override;
    virtual void render(Game& game) override;

    void set_color(const Color& color);
    void set_text(const std::string& text);

protected:
    FontPtr font_;
    std::string text_;
    Color color_;
};

class TWODCPP_EXPORT Button : public Label
{
public:
    Button(const std::string& text, Widget* parent = nullptr);

    virtual void update(Game& game) override;
    virtual void render(Game& game) override;
};

class TWODCPP_EXPORT Select : public Widget
{
public:
    Select(Widget* parent = nullptr);

    virtual void update(Game& game) override;
    virtual void render(Game& game) override;

    void add_option(const std::string& opt);

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

    virtual void render(Game& game) override;

    float step() const { return step_; }
    void setStep(float step) { step_ = std::min(step, 1.0f); }
    float value() const { return value_; }
    void setValue(float value) { value_ = std::min(value, 1.0f); }

private:
    void calculate() override;

private:
    HLayout layout_;
    Button left_;
    Button right_;
    Rectangle outline_;
    Rectangle rect_;
    float step_ = 0;
    float value_ = 0;
};
