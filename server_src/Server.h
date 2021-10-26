#ifndef SERVER_H
#define SERVER_H

#include "../common_src/Socket.h"
#include "QueuesMap.h"
#include "ServerThread.h"
#include <vector>

class Server {
private:
    Socket sk;
    QueuesMap queuesMap;
    std::vector<ServerThread> threads;

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
