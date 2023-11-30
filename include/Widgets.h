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

using WidgetPtr = Ptr<Widget>;
using LabelPtr = Ptr<Label>;
using SelectPtr = Ptr<Select>;
using GridPtr = Ptr<Grid>;
using SliderPtr = Ptr<Slider>;
using ButtonPtr = Ptr<Button>;

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

    virtual void update(Game& game);
    virtual void render(Game& game);

    const Point& pos() const { return pos_; }
    void setPos(const Point& pos) { pos_ = pos; }

    const Size& size() const { return size_; }
    void setSize(const Size& size) { size_ = size; }

    const Size& spacing() const { return spacing_; }
    void setSpacing(const Size& spacing) { spacing_ = spacing; }

    bool inFocus() const { return inFocus_; }
    bool enabled() const { return enabled_; }
    void setEnabled(bool enabled) { enabled_ = enabled; }

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

class TWODCPP_EXPORT Grid : public Widget
{
public:
    Grid(int cells_w, int cells_h, Widget* parent = nullptr);
    ~Grid();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void set(int row, int col, Widget* w);

private:
    int cells_w = 0;
    int cells_h = 0;
};

class TWODCPP_EXPORT Label : public Widget
{
public:
    Label(const std::string& text, Widget* parent = 0);
    virtual ~Label();

    virtual void update(Game& game);
    virtual void render(Game& game);

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
    virtual ~Button();

    virtual void update(Game& game);
    virtual void render(Game& game);
};

class TWODCPP_EXPORT Select : public Widget
{
public:
    Select(Widget* parent = nullptr);
    ~Select();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void add_option(const std::string& opt);

private:
    Button left;
    Button right;
    std::vector<LabelPtr> options;
    int current_option = 0;
};

class TWODCPP_EXPORT Slider : public Widget
{
public:
    Slider(float step, float value, Widget* parent = nullptr);
    ~Slider();

    virtual void update(Game& game);
    virtual void render(Game& game);

    float step() const { return step_; }
    void setStep(float step) { step_ = std::min(step, 1.0f); }
    float value() const { return value_; }
    void setValue(float value) { value_ = std::min(value, 1.0f); }

private:
    Button left_;
    Button right_;
    Rect sliderOutline_;
    float step_ = 0;
    float value_ = 0;
};