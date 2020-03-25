#ifndef COROUTINES_ACTORTHREAD_H
#define COROUTINES_ACTORTHREAD_H

#include <functional>
#include <vector>

class Actor;

class ActorThread {
public:

    ~ActorThread();

public:

    void addActor(Actor &actor);

    void run();

private:

    std::vector<std::reference_wrapper<Actor>> actors;

};

#endif //COROUTINES_ACTORTHREAD_H
