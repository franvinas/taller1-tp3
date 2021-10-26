#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H

#include <deque>
#include <string>

class Queue {
private:
    std::deque<std::string> queue;
public:
    void push(const std::string &message);
    std::string pop();
};


#endif
