#include "ActorThread.h"

#include "Actor.h"

ActorThread::~ActorThread() = default;

void ActorThread::addActor(Actor &actor) {
    actors.emplace_back(actor);
}

void ActorThread::run() {
    while (true) {
        for (Actor &actor: actors) {
            actor.tryRunTask();
        }
    }
}
