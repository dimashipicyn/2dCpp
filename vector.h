#ifndef VECTOR_H
#define VECTOR_H

template<class T>
struct Vec2
{
    T x;
    T y;
    
    Vec2()
        : x()
        , y()
    {
        
    }
    
    Vec2(const T& x, const T& y)
        : x(x)
        , y(y)
    {
        
    }
    
    double length() const;
    Vec2<T> add(const Vec2<T>& vec2) const;
    Vec2<T> sub(const Vec2<T>& vec2) const;
    Vec2<T> scalar(float n) const;
    double scalar(const Vec2<T>& vec2) const;
    double distance(const Vec2<T>& vec2) const;
    Vec2<T> normalize() const;
    Vec2<T> rotate(float angle) const;
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

#endif

