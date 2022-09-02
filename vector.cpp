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
    return {x * n, y * n};
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
    return {x * len, y * len};
}

template<class T>
Vec2<T> Vec2<T>::rotate(float angle)
{
    val rv;

    rv.x = x * cos(angle) - y * sin(angle);
    rv.y = x * sin(angle) + y * cos(angle);

    return rv;
}
