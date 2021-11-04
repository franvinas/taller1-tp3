#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H

#include <deque>
#include <string>

class Queue {
private:
    std::deque<std::string> queue;
public:
    /*
     *  Encola el mensaje
     */
    void push(const std::string &message);
    /*
     *  Desencola un mensaje y lo retorna
     */
    std::string pop();
};

#endif
