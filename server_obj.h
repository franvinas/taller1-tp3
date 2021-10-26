#ifndef SERVER_H
#define SERVER_H

#include "common_proxy.h"

class Server {
private:
    Proxy proxy;

public:
    /*
     *  Constructor
     */
    Server(const char *service);
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