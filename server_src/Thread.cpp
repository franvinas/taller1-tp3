#include "Thread.h"
#include <iostream>
#include <utility>

Thread::Thread() {}

// This template method catches exceptions (they can't be caught from main)
void Thread::runExpecting() {
    try {
        run();
    } catch(const std::exception &e) {
        std::cerr << "Exception caught in a thread: '" 
                  << e.what() << "'" << std::endl;
    } catch(...) {
        std::cerr << "Unknown error caught in thread" << std::endl;
    }
}

void Thread::start() {
    t = std::thread(&Thread::runExpecting, this);
}

void Thread::join() {
    t.join();
}

Thread::Thread(Thread &&other) : t(std::move(other.t)) {}

Thread& Thread::operator=(Thread&& other) {
    if (this == &other)
        return *this;
    
    this->t = std::move(other.t);

    return *this;
}
