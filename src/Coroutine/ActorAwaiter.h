#ifndef COROUTINES_ACTORAWAITER_H
#define COROUTINES_ACTORAWAITER_H

#include <coroutine>
#include <memory>
#include <optional>
#include <functional>

template<typename... T>
struct ActorAwaiter {

    std::tuple<T...> values;

    std::function<void(std::tuple<T...> &values, std::coroutine_handle<>)> storeHandler;

    ActorAwaiter(const std::function<void(std::tuple<T...> &values, std::coroutine_handle<>)> &storeHandler)
        : storeHandler(storeHandler)
    {}

    ActorAwaiter(const ActorAwaiter &) = delete;
    ActorAwaiter& operator=(const ActorAwaiter &) = delete;
    ActorAwaiter(ActorAwaiter &&) = delete;
    ActorAwaiter& operator=(ActorAwaiter &&) = delete;

    ~ActorAwaiter() {
    }

    bool await_ready() const noexcept {
        return false;
    }

    void await_suspend(std::coroutine_handle<> coro) noexcept {
        std::invoke(storeHandler, std::ref(values), coro);
    }

    template<bool B = true, size_t len = sizeof...(T), std::enable_if_t<len == 0 && B, int> = 0>
    void await_resume() noexcept {

    }

    template<bool B = true, size_t len = sizeof...(T), std::enable_if_t<len == 1 && B, int> = 0>
    auto await_resume() noexcept {
        return std::get<0>(values);
    }

    template<bool B = true, size_t len = sizeof...(T), std::enable_if_t<len != 1 && len != 0 && B, int> = 0>
    std::tuple<T...> await_resume() noexcept {
        return values;
    }
};

#endif //COROUTINES_ACTORAWAITER_H
