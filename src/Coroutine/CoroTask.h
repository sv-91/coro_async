#ifndef COROUTINES_COROTASK_H
#define COROUTINES_COROTASK_H

#include <coroutine>
#include <exception>
#include <functional>
#include <iostream>

template <typename T = void>
struct CoroTask {
    struct promise_type {
        T result;
        std::coroutine_handle<> waiter;

        auto get_return_object() {
            return CoroTask{*this};
        }

        void return_value(T value) {
            result = value;
        }

        void unhandled_exception() {
            std::terminate();
        }

        std::suspend_always initial_suspend() {
            return {};
        }

        auto final_suspend() {
            struct final_awaiter {
                bool await_ready() {
                    return false;
                }

                void await_resume() {}

                auto await_suspend(std::coroutine_handle<promise_type> me) {
                    return me.promise().waiter;
                }
            };
            return final_awaiter{};
        }
    };

    CoroTask(CoroTask &&) = delete;
    CoroTask& operator=(CoroTask&&) = delete;
    CoroTask(const CoroTask&) = delete;
    CoroTask& operator=(const CoroTask&) = delete;

    ~CoroTask() {
        if (h) {
            h.destroy();
        }
    }

    explicit CoroTask(promise_type & p)
        : h(std::coroutine_handle<promise_type>::from_promise(p))
    {}

    bool await_ready() {
        return false;
    }

    T await_resume() {
        auto &result = h.promise().result;
        return result;
    }

    void await_suspend(std::coroutine_handle<> waiter) {
        h.promise().waiter = waiter;
        h.resume();
    }

private:
    std::coroutine_handle<promise_type> h;
};

template<typename... T>
struct CoroTask2 {
    struct promise_type {
        std::tuple<T...> savedValues;
        std::function<void(std::tuple<T...> &results, std::coroutine_handle<>)> storeHandler;
        std::coroutine_handle<> waiter;

        auto get_return_object() {
            return CoroTask2{*this};
        }

        void return_value(std::function<void(std::tuple<T...> &results, std::coroutine_handle<>)> value) {
            storeHandler = value;
            storeHandler(savedValues, waiter);
        }

        void unhandled_exception() {
            std::terminate();
        }

        std::suspend_always initial_suspend() {
            return {};
        }

        auto final_suspend() {
            return std::suspend_always{};
        }
    };

    CoroTask2(CoroTask2 &&) = delete;
    CoroTask2& operator=(CoroTask2&&) = delete;
    CoroTask2(const CoroTask2&) = delete;
    CoroTask2& operator=(const CoroTask2&) = delete;

    ~CoroTask2() {
        if (h) {
            h.destroy();
        }
    }

    explicit CoroTask2(promise_type & p)
        : h(std::coroutine_handle<promise_type>::from_promise(p))
    {}

    bool await_ready() {
        return false;
    }

    template<bool B = true, size_t len = sizeof...(T), std::enable_if_t<len == 0 && B, int> = 0>
    void await_resume() noexcept {

    }

    template<bool B = true, size_t len = sizeof...(T), std::enable_if_t<len == 1 && B, int> = 0>
    auto await_resume() noexcept {
        const auto &result = h.promise().savedValues;
        return std::get<0>(result);
    }

    template<bool B = true, size_t len = sizeof...(T), std::enable_if_t<len != 1 && len != 0 && B, int> = 0>
    std::tuple<T...> await_resume() noexcept {
        const auto &result = h.promise().savedValues;
        return result;
    }

    void await_suspend(std::coroutine_handle<> waiter) {
        h.promise().waiter = waiter;
        h.resume();
    }

private:
    std::coroutine_handle<promise_type> h;
};

#endif //COROUTINES_COROTASK_H
