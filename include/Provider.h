#pragma once

#include "Export.h"

#include <cassert>

class TWODCPP_EXPORT Provider
{
    template <class T>
    T** service()
    {
        static T* serv = nullptr;
        return &serv;
    }

public:
    static Provider& get();

    template <class T>
    void provide(T* serv) { *service<T>() = serv; }

    template<class T>
    T& get()
    {
        T* s = *service<T>();
        assert(s && "Empty service!");
        return *s;
    }

private:
    Provider() = default;
};