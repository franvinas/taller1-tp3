#ifndef SERVER_H
#define SERVER_H

#include "../common_src/Proxy.h"
#include "QueuesMap.h"

class Server {
private:
    Proxy proxy;
    QueuesMap queuesMap;

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
