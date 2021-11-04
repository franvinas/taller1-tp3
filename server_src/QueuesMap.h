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
    /*
     *  Si no existe una cola con el nombre 'queue_name', entonces la crea
     */
    void define(const std::string &queue_name);
    /*
     *  Encola en la cola de nombre 'queue_name' el mensaje 'message'
     */
    void push(const std::string &queue_name, const std::string &message);
    /*
     *  Desencola un mensaje de la cola de nombre 'queue_name' y lo retorna
     */
    std::string pop(const std::string &queue_name);
};

#endif
