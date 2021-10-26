#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include "Thread.h"
#include "Protocol.h"
#include "QueuesMap.h"

/*
 *  ServerThread hereda de Thread e implementa run().
 *  Todas las instancias ServerThread tienen una referencia a la taskQueue y
 *  al dataset. Notar que tanto el taskQueue como el dataset son los 
 *  mismos para todas los ServerThreads.
 */
class ServerThread: public Thread {
private:
    Protocol protocol;    
    QueuesMap &queuesMap;
    
protected:
    /*
     *  Se busca por el siguiente Task que no fue analizado completamente
     *  y se aplica la operacion correspondiente para dicho Task en la
     *  siguiente particion. Una vez que ya fueron analizados todos los
     *  Tasks la funcion retorna.
     */
    void run() override;

public:
    /*
     *  Constructor
     */
    ServerThread(Socket &peer_sk, QueuesMap &queuesMap);
};

#endif
