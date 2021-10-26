#include "common_command.h"
#include <iostream>

Command::~Command() {}

Define::Define(const std::string &queue_name) {
    this->queue_name = queue_name;
}

void Define::operate() {
    std::cout << "Define " << this->queue_name << "\n";
}

Push::Push(const std::string &queue_name, const std::string &message) {
    this->queue_name = queue_name;
    this->message = message;
}

void Push::operate() {
    std::cout << "Push " << this->queue_name << " " << this->message << "\n";
}

Pop::Pop(const std::string &queue_name) {
    this->queue_name = queue_name;
}

void Pop::operate() {
    std::cout << "Pop\n";
}
