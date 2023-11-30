#pragma once

#include <array>
#include <list>
#include <functional>
#include <cassert>

template <class T, class SignalEnum, SignalEnum signalEnumSize>
class Signal
{
public:
	using Callback = std::function<void(const T&)>;
    using CallbackWithoutArg = std::function<void()>;

private:
    struct Ctx
    {
        void* owner;
        CallbackWithoutArg callback;
    };

public:
    template<class Owner>
    using CallbackMf = void (Owner::*)(const T&);
    
    template <class Owner>
    void on(SignalEnum signal, Owner* owner, CallbackMf<Owner> callback)
	{
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { owner, [owner, callback, this]()
            { std::invoke(callback, owner, *static_cast<T*>(this)); }
        });
	}

    template <class Owner>
	using CallbackMfWithoutArg = void(Owner::*)();

    template<class Owner>
    void on(SignalEnum signal, Owner* owner, CallbackMfWithoutArg<Owner> callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { owner, [owner, callback]()
            { std::invoke(callback, owner); } });
    }

    void on(SignalEnum signal, Callback callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { nullptr, [cb = std::move(callback), this]
            { std::invoke(cb, *static_cast<T*>(this)); } });
    }

    void on(SignalEnum signal, CallbackWithoutArg callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { nullptr, [cb = std::move(callback)]
            { std::invoke(cb); } });
    }

    void remove(SignalEnum signal, void* owner)
    {
        auto& list = functions_[(int)signal];
        list.erase(std::remove_if(list.begin(), list.end(), [owner](const Ctx& ctx)
                       { return ctx.owner == owner; }),
            list.end());
    }

	void invoke(SignalEnum signal)
    {
        for (Ctx& ctx : functions_[(int)signal])
        {
            ctx.callback();
        }
    }

private:
	using CtxList = std::list<Ctx>;
	std::array<CtxList, (int)signalEnumSize> functions_;
};