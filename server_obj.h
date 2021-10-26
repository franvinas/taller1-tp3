#ifndef SERVER_H
#define SERVER_H

#include "common_proxy.h"
#include "server_queues_map.h"

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
