#include "server_queue.h"
#include <string>

void Queue::push(const std::string &message) {
    this->queue.push_back(message);
}

std::string Queue::pop() {
    if (this->queue.empty()) throw -1;
    std::string message = std::move(this->queue.front());
    this->queue.pop_front();
    return message;
}
