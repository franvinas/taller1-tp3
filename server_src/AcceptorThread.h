#ifndef ACCEPTOR_THREAD_H
#define ACCEPTOR_THREAD_H

#include "Thread.h"
#include "QueuesMap.h"
#include "ServerThread.h"
#include "../common_src/Socket.h"
#include <list>

/*
 *  AcceptorThread hereda de Thread e implementa run().
 */
class AcceptorThread: public Thread {
private:
    Socket &sk;
    QueuesMap queuesMap;
    std::list<ServerThread> threads;
    
protected:
    /*
     *  
     */
    void run() override;

public:
    /*
     *  Constructor
     */
    explicit AcceptorThread(Socket &sk);
    /*
     *  Constructor por copia
     */
    AcceptorThread(const AcceptorThread&) = delete;
    /*
     *  Constructor por movimiento
     */
    AcceptorThread(AcceptorThread &&other) = delete;
    /*
     *  Cierra las conexiones con los clientes y corta su ejecucion
     */
    void stop();
};

#endif
