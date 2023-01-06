#include "stdafx.h"

#include "vector.h"

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <cmath>

template<class T>
double Vec2<T>::length() const
{
    return sqrt(x * x + y * y);
}

template<class T>
Vec2<T> Vec2<T>::add(const Vec2<T>& vec2) const
{
    return {x + vec2.x, y + vec2.y};
}

template<class T>
Vec2<T> Vec2<T>::sub(const Vec2<T>& vec2) const
{
    return {x - vec2.x, y - vec2.y};
}

template<class T>
Vec2<T> Vec2<T>::scalar(float n) const
{
    return {static_cast<T>(x * n), static_cast<T>(y * n)};
}

template<class T>
double Vec2<T>::scalar(const Vec2<T>& vec2) const
{
    return x * vec2.x + y * vec2.y;
}

template<class T>
double Vec2<T>::distance(const Vec2<T>& vec2) const
{
    return sub(vec2).length();
}

template<class T>
Vec2<T> Vec2<T>::normalize() const
{
    double len = length();
	if (len > 0) {
		return {static_cast<T>(x / len), static_cast<T>(y / len)};
	}
	return *this;
}

namespace {
    float deg2rad(float angle) {
        return angle * (M_PI / 180);
    }
}

template<class T>
Vec2<T> Vec2<T>::rotate(float angle) const
{
    Vec2<T> rv;
    float rad = deg2rad(angle);

    rv.x = x * cos(rad) - y * sin(rad);
    rv.y = x * sin(rad) + y * cos(rad);

    return rv;
}

template<class T>
Vec2<T> Vec2<T>::operator-() const {
	return Vec2<T>(-x, -y);
}

template struct Vec2<int>;
template struct Vec2<float>;
template struct Vec2<double>;
