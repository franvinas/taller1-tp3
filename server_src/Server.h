#ifndef SERVER_H
#define SERVER_H

#include "../common_src/Socket.h"
#include "QueuesMap.h"
#include "ServerThread.h"
#include <deque>

class Server {
private:
    Socket sk;
    QueuesMap queuesMap;
    std::deque<ServerThread> threads;

public:
    /*
     *  Constructor
     */
    explicit Server(const char *service);
    /*
     *  
     */
    void run();
    /*
     *  Destructor
     */
    // ~Server();
};

#endif
