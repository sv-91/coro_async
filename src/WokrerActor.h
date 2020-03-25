#ifndef COROUTINES_WOKRERACTOR_H
#define COROUTINES_WOKRERACTOR_H

#include "Actors/Actor.h"

#include "Coroutine/CoroTask.h"

class ABActor;

class WokrerActor: public Actor {
public:

    WokrerActor(ABActor &actor)
        : Actor("WorkerActor")
        , abActor(actor)
    {}

public:

    void work();

    void workCoro();

    void workCoro2();

private:

    void workProcess();

    void workCoroProcess();

    void workCoro2Process();

private:

    CoroTask<std::pair<int, int>> readAB();

private:

    ABActor &abActor;
};

#endif //COROUTINES_WOKRERACTOR_H
