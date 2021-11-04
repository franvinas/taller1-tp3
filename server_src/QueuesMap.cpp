#include "QueuesMap.h"
#include <string>

/***********************
    Metodos publicos
************************/

void QueuesMap::define(const std::string &queue_name) {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->map.insert({queue_name, Queue()});
}

void QueuesMap::push(const std::string &queue_name, 
                     const std::string &message) {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->map[queue_name].push(message);
}

std::string QueuesMap::pop(const std::string &queue_name) {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->map[queue_name].pop();
}
