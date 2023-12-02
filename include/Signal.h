#pragma once

#include <array>
#include <list>
#include <functional>
#include <cassert>

class SignalBase
{
public:
    virtual ~SignalBase()
    {
        for (SignalBase* s : connected_)
        {
            if (s != this)
            {
                s->remove(this);
            }
        }
    }

    virtual void remove(SignalBase* owner) = 0;

protected:
    std::list<SignalBase*> connected_;
};

template <class T, class SignalEnum, SignalEnum signalEnumSize>
class Signal : public SignalBase
{
public:
    using Callback = std::function<void(const T*)>;
    using CallbackWithoutArg = std::function<void()>;

private:
    struct Ctx
    {
        SignalBase* owner;
        CallbackWithoutArg callback;
    };

public:
    template<class Owner>
    using CallbackMf = void (Owner::*)(const T*);
    
    template <class Owner>
    void on(SignalEnum signal, SignalBase* owner, CallbackMf<Owner> callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { owner, [owner, callback, this]()
            { std::invoke(callback, owner, *static_cast<T*>(this)); }
        });
        owner->connected_.push_front(this);
    }

    template <class Owner>
    using CallbackMfWithoutArg = void(Owner::*)();

    template<class Owner>
    void on(SignalEnum signal, SignalBase* owner, CallbackMfWithoutArg<Owner> callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { owner, [owner, callback]()
            { std::invoke(callback, static_cast<Owner*>(owner)); } });
    }

    void on(SignalEnum signal, Callback callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { nullptr, [cb = std::move(callback), this]
            { std::invoke(cb, static_cast<T*>(this)); } });
    }

    void on(SignalEnum signal, SignalBase* owner, Callback callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { owner, [cb = std::move(callback), this]
            { std::invoke(cb, static_cast<T*>(this)); } });
    }

    void on(SignalEnum signal, CallbackWithoutArg callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { nullptr, std::move(callback) });
    }

    void on(SignalEnum signal, SignalBase* owner, CallbackWithoutArg callback)
    {
        assert((int)signal < functions_.size() && (int)signal >= 0 && "Wrong signal!");
        functions_[(int)signal].push_front(Ctx { owner, std::move(callback) });
    }

    void remove(SignalEnum signal, SignalBase* owner)
    {
        auto& list = functions_[(int)signal];
        list.erase(std::remove_if(list.begin(), list.end(), [owner](const Ctx& ctx)
                       { return ctx.owner == owner; }),
            list.end());
    }

    void remove(SignalBase* owner) override
    {
        for (int i = 0; i < functions_.size(); ++i)
        {
            remove((SignalEnum)i, owner);
        }
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
