#pragma once

template <class T, class U = void (*)(T), U Destroyer = nullptr>
class RaiiWrapper
{
public:
    RaiiWrapper(/*Destroyer destroyer*/)
        /*: destroyer_ { destroyer }*/
    {
    }

    RaiiWrapper(T object/*, Destroyer destroyer*/)
        : object_ { object }
        /*, destroyer_ { destroyer }*/
    {
    }

    ~RaiiWrapper()
    {
        if (Destroyer)
        {
            Destroyer(object_);
        }
    }

    T get()
    {
        return object_;
    }

    T* operator&()
    {
        return &object_;
    }

    T operator->()
    {
        return get();
    }

private:
    T object_ {};
};