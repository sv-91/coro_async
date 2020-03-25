#include "WokrerActor.h"

#include "ABActor.h"

#include <iostream>

#include "Coroutine/ActorAwaiter.h"
#include "Coroutine/ActorResumable.h"

void WokrerActor::work() {
    addTask(std::bind(&WokrerActor::workProcess, this));
}

void WokrerActor::workCoro() {
    addTask(std::bind(&WokrerActor::workCoroProcess, this));
}

void WokrerActor::workCoro2() {
    addTask(std::bind(&WokrerActor::workCoro2Process, this));
}

void WokrerActor::workProcess() {
    abActor.getA(ABActor::GetACallback(*this, [this](int a) {
        abActor.getB(ABActor::GetBCallback(*this, [a, this](int b) {
            abActor.saveAB(a - b, a + b, ABActor::SaveABCallback(*this, [this](){
                abActor.getA(ABActor::GetACallback(*this, [this](int a) {
                    abActor.getB(ABActor::GetBCallback(*this, [a, this](int b) {
                        std::cout << "Result " << a << " " << b << std::endl;
                    }));
                }));
            }));
        }));
    }));
}

void WokrerActor::workCoroProcess() {
    const auto coroutine = [](WokrerActor &actor) -> ActorResumable {
        const int a = co_await actor.abActor.getAAsync(actor);
        const int b = co_await actor.abActor.getBAsync(actor);
        co_await actor.abActor.saveABAsync(actor, a - b, a + b);
        const int newA = co_await actor.abActor.getAAsync(actor);
        const int newB = co_await actor.abActor.getBAsync(actor);
        std::cout << "Result " << newA << " " << newB << std::endl;
        const auto [aHalf, bHalf] = co_await actor.abActor.getABHalfAsync(actor);
        std::cout << "ABHalf " << aHalf << " " << bHalf << std::endl;
    };

    coroutine(*this);
}

CoroTask<std::pair<int, int>> WokrerActor::readAB() {
    const int a = co_await abActor.getAAsync2(*this);
    const int b = co_await abActor.getBAsync2(*this);
    co_return std::make_pair(a, b);
}

void WokrerActor::workCoro2Process() {
    const auto coroutine = [](WokrerActor &actor) -> ActorResumable {
        const auto [a, b] = co_await actor.readAB();
        co_await actor.abActor.saveABAsync2(actor, a - b, a + b);
        const auto [newA, newB] = co_await actor.readAB();
        std::cout << "Result2 " << newA << " " << newB << " " << a << " " << b << std::endl;
    };

    coroutine(*this);
}
