#ifndef CLIENT_H
#define CLIENT_H

#include "../common_src/common_proxy.h"

class Client {
private:
    Proxy proxy;
    void define();
    void push();
    void pop();

public:
    /*
     *  Constructor
     */
    Client(const char *host, const char *service);
    /*
     *  
     */
    void run();
    /*
     *  Destructor
     */
    // ~Client();
};

#endif
