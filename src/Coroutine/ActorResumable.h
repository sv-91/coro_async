#ifndef COROUTINES_ACTORRESUMABLE_H
#define COROUTINES_ACTORRESUMABLE_H

#include <coroutine>
#include <exception>

struct ActorResumable {
    struct promise_type {
        using coro_handle = std::coroutine_handle<promise_type>;

        promise_type() {
        }

        auto get_return_object() {
            return coro_handle::from_promise(*this);
        }

        auto initial_suspend() {
            return std::suspend_never();
        }

        auto final_suspend() {
            return std::suspend_never();
        }

        void return_void() {

        }

        void unhandled_exception() {
            std::terminate();
        }

        ~promise_type() {

        }
    };

    using coro_handle = std::coroutine_handle<promise_type>;

    ActorResumable(coro_handle handle) {}

    ~ActorResumable() {
    }

};

#endif //COROUTINES_ACTORRESUMABLE_H
