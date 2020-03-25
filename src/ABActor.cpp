#include "ABActor.h"

#include <iostream>

void ABActor::getA(const GetACallback &callback) {
    addTask(std::bind(&ABActor::getAProcess, this, callback));
}

void ABActor::getAProcess(const ABActor::GetACallback &callback) {
    std::invoke(callback, a);
}

void ABActor::getB(const GetBCallback &callback) {
    addTask(std::bind(&ABActor::getBProcess, this, callback));
}

void ABActor::getBProcess(const ABActor::GetBCallback &callback) {
    std::invoke(callback, b);
}

void ABActor::saveAB(int a, int b, const SaveABCallback &callback) {
    addTask(std::bind(&ABActor::saveABProcess, this, a, b, callback));
}

void ABActor::getABHalf(const GetABHalfCallback &callback) {
    addTask(std::bind(&ABActor::getABHalfProcess, this, callback));
}

void ABActor::getABHalfProcess(const ABActor::GetABHalfCallback &callback) {
    std::invoke(callback, a / 2, b / 2);
}

void ABActor::saveABProcess(int a, int b, const ABActor::SaveABCallback &callback) {
    this->a = a;
    this->b = b;
    std::invoke(callback);
}

template<typename MakeCallback, typename... ReturnArgs, typename Func>
static auto makeCoroCallback(const Func &func, Actor &returnCallback) {
    return [&returnCallback, func](auto &values, std::coroutine_handle<> coro) {
        auto callback = MakeCallback(returnCallback, [&values, coro](ReturnArgs&& ...result) {
            values = std::make_tuple(std::forward<ReturnArgs>(result)...);
            std::invoke(coro);
        });
        func(callback);
    };
}

template<typename MakeCallback, typename... ReturnArgs, typename Func>
static ActorAwaiter<ReturnArgs...> makeActorAwaiter(const Func &func, Actor &returnCallback) {
    const auto storeCoroToQueue = makeCoroCallback<MakeCallback, ReturnArgs...>(func, returnCallback);
    return ActorAwaiter<ReturnArgs...>(storeCoroToQueue);
}

ActorAwaiter<int> ABActor::getAAsync(Actor &returnCallback) {
    return makeActorAwaiter<GetACallback, int>(std::bind(&ABActor::getA, this, std::placeholders::_1), returnCallback);
}

ActorAwaiter<int> ABActor::getBAsync(Actor &returnCallback) {
    return makeActorAwaiter<GetBCallback, int>(std::bind(&ABActor::getB, this, std::placeholders::_1), returnCallback);
}

ActorAwaiter<> ABActor::saveABAsync(Actor &returnCallback, int a, int b) {
    return makeActorAwaiter<SaveABCallback>(std::bind(&ABActor::saveAB, this, a, b, std::placeholders::_1), returnCallback);
}

ActorAwaiter<int, int> ABActor::getABHalfAsync(Actor &returnCallback) {
    return makeActorAwaiter<GetABHalfCallback, int, int>(std::bind(&ABActor::getABHalf, this, std::placeholders::_1), returnCallback);
}

CoroTask2<int> ABActor::getAAsync2(Actor &returnCallback) {
    co_return makeCoroCallback<GetACallback, int>(std::bind(&ABActor::getA, this, std::placeholders::_1), returnCallback);
}

CoroTask2<int> ABActor::getBAsync2(Actor &returnCallback) {
    co_return makeCoroCallback<GetBCallback, int>(std::bind(&ABActor::getB, this, std::placeholders::_1), returnCallback);
}

CoroTask2<> ABActor::saveABAsync2(Actor &returnCallback, int a, int b) {
    co_return makeCoroCallback<SaveABCallback>(std::bind(&ABActor::saveAB, this, a, b, std::placeholders::_1), returnCallback);
}
