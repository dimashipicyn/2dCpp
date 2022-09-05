#ifndef VECTOR_H
#define VECTOR_H

template<class T>
struct Vec2
{
    using ref = Vec2<T>&;
    using ptr = Vec2<T>*;
    using val = Vec2<T>;
    
    T x;
    T y;
    
    double length();
    val add(ref vec2);
    val sub(ref vec2);
    val scalar(float n);
    double scalar(ref vec2);
    double distance(ref vec2);
    val normalize();
    val rotate(float angle);
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

#endif

