#include "Queue.h"
#include <string>
#include <utility>
#include <stdexcept>

/***********************
    Metodos publicos
************************/

void Queue::push(const std::string &message) {
    this->queue.push_back(message);
}

std::string Queue::pop() {
    if (this->queue.empty()) throw std::runtime_error("Error: Queue is empty");
    std::string message = std::move(this->queue.front());
    this->queue.pop_front();
    return message;
}

bool Queue::empty() const {
    return this->queue.empty();
}
