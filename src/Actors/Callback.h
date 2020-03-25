#ifndef COROUTINES_CALLBACK_H
#define COROUTINES_CALLBACK_H

#include <functional>

#include "Actor.h"

template<typename C>
class Callback {
public:

    template<typename Functor>
    Callback(Actor &sender, const Functor &callback)
        : sender(sender)
        , callback(callback)
    {}

public:

    template<typename ...Args>
    void operator() (Args&& ...args) const {
        sender.addTask(std::bind(callback, std::forward<Args>(args)...));
    }

private:

    Actor &sender;

    std::function<C> callback;

};

#endif //COROUTINES_CALLBACK_H
