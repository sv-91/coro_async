#ifndef COROUTINES_ABACTOR_H
#define COROUTINES_ABACTOR_H

#include "Actors/Actor.h"
#include "Actors/Callback.h"

#include "Coroutine/ActorAwaiter.h"
#include "Coroutine/CoroTask.h"

class ABActor: public Actor {
public:

    using GetACallback = Callback<void(int result)>;

    using GetBCallback = Callback<void(int result)>;

    using SaveABCallback = Callback<void()>;

    using GetABHalfCallback = Callback<void(int a, int b)>;

public:

    ABActor()
        : Actor("ABActor")
    {}

public:

    void getA(const GetACallback &callback);

    void getB(const GetBCallback &callback);

    void saveAB(int a, int b, const SaveABCallback &callback);

    void getABHalf(const GetABHalfCallback &callback);

public:

    ActorAwaiter<int> getAAsync(Actor &returnCallback);

    ActorAwaiter<int> getBAsync(Actor &returnCallback);

    ActorAwaiter<> saveABAsync(Actor &returnCallback, int a, int b);

    ActorAwaiter<int, int> getABHalfAsync(Actor &returnCallback);

    CoroTask2<int> getAAsync2(Actor &returnCallback);

    CoroTask2<int> getBAsync2(Actor &returnCallback);

    CoroTask2<> saveABAsync2(Actor &returnCallback, int a, int b);

private:

    void getAProcess(const GetACallback &callback);

    void getBProcess(const GetBCallback &callback);

    void saveABProcess(int a, int b, const SaveABCallback &callback);

    void getABHalfProcess(const GetABHalfCallback &callback);

private:

    int a = 10;

    int b = 20;

};

#endif //COROUTINES_ABACTOR_H
