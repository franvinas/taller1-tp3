#include "QueuesMap.h"
#include <string>

/***********************
    Metodos publicos
************************/

void QueuesMap::define(const std::string &queue_name) {
    std::unique_lock<std::mutex> Uniquelock(this->mutex);
    this->map.insert({queue_name, Queue()});
}

void QueuesMap::push(const std::string &queue_name, 
                     const std::string &message) {
    std::unique_lock<std::mutex> Uniquelock(this->mutex);
    this->map[queue_name].push(message);
    cv.notify_all();
}

std::string QueuesMap::pop(const std::string &queue_name) {
    std::unique_lock<std::mutex> uniqueLock(this->mutex);
    while (this->map[queue_name].empty())
        cv.wait(uniqueLock);
    return this->map[queue_name].pop();
}
