#ifndef QUEUES_MAP_H
#define QUEUES_MAP_H

#include "Queue.h"
#include <map>
#include <string>
#include <mutex>

class QueuesMap {
private:
    std::map<std::string,Queue> map;
    std::mutex mutex;
public:
    // QueuesMap();
    void define(const std::string &queue_name);
    void push(const std::string &queue_name, const std::string &message);
    std::string pop(const std::string &queue_name);
};

#endif
