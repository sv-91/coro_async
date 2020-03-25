#include "Actor.h"

#include <iostream>

Actor::Actor(const std::string &name)
    : name(name)
{}

Actor::~Actor() = default;

void Actor::addTask(const Task &task) {
    std::cout << "Add task to actor " << name << " " << this << std::endl;
    std::lock_guard lock(mutex);
    queue.push(task);
}

void Actor::tryRunTask() {
    std::unique_lock lock(mutex);
    if (queue.empty()) {
        return;
    }

    std::cout << "Run task in actor " << name << " " << this << std::endl;

    const Task task = queue.front();
    queue.pop();
    lock.unlock();

    std::invoke(task);
}
