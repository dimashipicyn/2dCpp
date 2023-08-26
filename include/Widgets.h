#pragma once

#include "graphics.h"
#include "Common.hpp"
#include "Font.h"
#include "Sprite.h"

#include <fstream>
#include <functional>
#include <string>
#include <vector>

class Game;
class Widget;
class WidgetLabel;
class WidgetSelect;
class WidgetGrid;
class WidgetSlider;
class WidgetButton;

using WidgetPtr = Ptr<Widget>;
using WidgetLabelPtr = Ptr<WidgetLabel>;
using WidgetSelectPtr = Ptr<WidgetSelect>;
using WidgetGridPtr = Ptr<WidgetGrid>;
using WidgetSliderPtr = Ptr<WidgetSlider>;
using WidgetButtonPtr = Ptr<WidgetButton>;

class Widget
{
public:
    Widget(const std::string& name, int x = 0, int y = 0);
    virtual ~Widget();

    virtual void update(Game& game) = 0;
    virtual void render(Game& game) = 0;

    void on_click(Widget& w);
    void on_enter(Widget& w);
    void on_leave(Widget& w);
    void on_changed(Widget& w);

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

class WidgetGrid : public Widget
{
public:
    WidgetGrid(const std::string& name, int x, int y, int cells_w, int cells_h, int cell_size_w, int cell_size_h);
    ~WidgetGrid();

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

class WidgetLabel : public Widget
{
public:
    WidgetLabel(FontPtr font, int x, int y, const std::string& name, const std::string& label);
    virtual ~WidgetLabel();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void set_color(const Color& color);
    void set_label(const std::string& label);

protected:
    FontPtr font;
    std::string label;
    Color color;
};

class WidgetButton : public Widget
{
public:
    WidgetButton(FontPtr font, int x, int y, const std::string& name, const std::string& text);
    virtual ~WidgetButton();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void set_color(const Color& color);
    void set_text(const std::string& text);

    using on_click_callback = std::function<void()>;
    void on_click(on_click_callback cb);

protected:
    FontPtr font;
    on_click_callback cb;
    std::string text;
    Color color;
    bool in_focus = false;
};

class WidgetSelect : public Widget
{
public:
    WidgetSelect(FontPtr font, int x, int y, const std::string& name);
    ~WidgetSelect();

    virtual void update(Game& game);
    virtual void render(Game& game);

    void set_color(const Color& color);
    void add_option(const std::string& opt);

private:
    WidgetButton left;
    WidgetButton right;
    WidgetLabel label;
    std::vector<std::string> options;
    int current_option = 0;
};

class WidgetSlider : public Widget
{
public:
    WidgetSlider(FontPtr font, int x, int y, int size, int step, const std::string& name);
    ~WidgetSlider();

    virtual void update(Game& game);
    virtual void render(Game& game);

private:
    WidgetButton left;
    WidgetButton right;
    int step = 0;
    int size = 0;
    int value = 0;
};