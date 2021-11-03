#ifndef SERVER_H
#define SERVER_H

#include "../common_src/Socket.h"

class Server {
private:
    Socket sk;

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
