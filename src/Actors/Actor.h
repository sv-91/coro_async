#ifndef COROUTINES_ACTOR_H
#define COROUTINES_ACTOR_H

#include <functional>
#include <mutex>
#include <queue>

class Actor {
public:

    using Task = std::function<void()>;

public:

    Actor(const std::string &name);

    virtual ~Actor();

public:

    void addTask(const Task &task);

    void tryRunTask();

private:

    std::string name;

    std::queue<Task> queue;

    mutable std::mutex mutex;
};

#endif //COROUTINES_ACTOR_H
