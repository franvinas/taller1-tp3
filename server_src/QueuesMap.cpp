#include "QueuesMap.h"
#include <string>

// QueuesMap::QueuesMap() {}

void QueuesMap::define(const std::string &queue_name) {
    if (this->map.count(queue_name) == 0)
        this->map[queue_name] = Queue();
}

void QueuesMap::push(const std::string &queue_name, 
                     const std::string &message) {
    this->map[queue_name].push(message);
}

std::string QueuesMap::pop(const std::string &queue_name) {
    return this->map[queue_name].pop();
}
