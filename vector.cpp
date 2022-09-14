#include "vector.h"

#include <cmath>

template<class T>
double Vec2<T>::length()
{
    return sqrt(x * x + y * y);
}

template<class T>
Vec2<T> Vec2<T>::add(ref vec2)
{
    return {x + vec2.x, y + vec2.y};
}

template<class T>
Vec2<T> Vec2<T>::sub(ref vec2)
{
    return {x - vec2.x, y - vec2.y};
}

template<class T>
Vec2<T> Vec2<T>::scalar(float n)
{
    return {static_cast<T>(x * n), static_cast<T>(y * n)};
}

template<class T>
double Vec2<T>::scalar(ref vec2)
{
    return x * vec2.x + y * vec2.y;
}

template<class T>
double Vec2<T>::distance(ref vec2)
{
    return sub(vec2).length();
}

template<class T>
Vec2<T> Vec2<T>::normalize()
{
    double len = length();
    return {static_cast<T>(x * len), static_cast<T>(y * len)};
}

namespace {
    float deg2rad(float angle) {
        return angle * (M_PI / 180);
    }
}

template<class T>
Vec2<T> Vec2<T>::rotate(float angle)
{
    val rv;
    float rad = deg2rad(angle);

    rv.x = x * cos(rad) - y * sin(rad);
    rv.y = x * sin(rad) + y * cos(rad);

    return rv;
}

template struct Vec2<int>;
template struct Vec2<float>;
template struct Vec2<double>;
