#include <chrono>
#include <iostream>

#include "ABActor.h"
#include "WokrerActor.h"
#include "Actors/ActorThread.h"

int main() {
    ABActor abActor;
    WokrerActor workerActor(abActor);

    ActorThread thread;
    thread.addActor(abActor);
    thread.addActor(workerActor);

    workerActor.workCoro2();

    thread.run();
}

