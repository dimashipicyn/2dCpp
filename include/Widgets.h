#pragma once

#include "Export.h"
#include "graphics.h"
#include "Common.hpp"
#include "Font.h"
#include "Sprite.h"
#include "Signal.h"

#include <fstream>
#include <functional>
#include <string>
#include <vector>

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
    Widget(const std::string& name, int x = 0, int y = 0);
    virtual ~Widget();

    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;

    int get_x();
    int get_y();
    int get_w();
    int get_h();

    void set_x(int x) {this->x = x;}
    void set_y(int y) {this->y = y;}
    void set_w(int w) {this->w = w;}
    void set_h(int h) {this->h = h;}

protected:
    std::string name;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

class TWODCPP_EXPORT Grid : public Widget
{
public:
    Grid(const std::string& name, int x, int y, int cells_w, int cells_h, int cell_size_w, int cell_size_h);
    ~Grid();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void set(int row, int col, WidgetPtr w);

private:
    std::vector<WidgetPtr> childs;
    int cells_w = 0;
    int cells_h = 0;
    int cell_size_w = 0;
    int cell_size_h = 0;
};

class TWODCPP_EXPORT Label : public Widget
{
public:
    Label(FontPtr font, int x, int y, const std::string& name, const std::string& label);
    virtual ~Label();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void set_color(const Color& color);
    void set_label(const std::string& label);

protected:
    FontPtr font;
    std::string label;
    Color color;
};

class TWODCPP_EXPORT Button : public Widget
{
public:
    Button(FontPtr font, int x, int y, const std::string& name, const std::string& text);
    virtual ~Button();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void set_color(const Color& color);
    void set_text(const std::string& text);

protected:
    FontPtr font;
    std::string text;
    Color color;
    bool in_focus = false;
};

class TWODCPP_EXPORT Select : public Widget
{
public:
    Select(FontPtr font, int x, int y, const std::string& name);
    ~Select();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void set_color(const Color& color);
    void add_option(const std::string& opt);

private:
    Button left;
    Button right;
    Label label;
    std::vector<std::string> options;
    int current_option = 0;
};

class TWODCPP_EXPORT Slider : public Widget
{
public:
    Slider(FontPtr font, int x, int y, int size, int step, const std::string& name);
    ~Slider();

    virtual void update(Game& game);
    virtual void render(Game& game);

private:
    Button left;
    Button right;
    int step = 0;
    int size = 0;
    int value = 0;
};