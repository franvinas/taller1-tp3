#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include "Thread.h"
#include "Protocol.h"
#include "QueuesMap.h"
#include <atomic>
#include <string>

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
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_running;
    void exec_cmd(const std::string &cmd, 
                  const std::string &queue_name, 
                  const std::string &message);
    
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
    /*
     *  Constructor por copia
     */
    ServerThread(const ServerThread &other) = delete;
    /*
     *  Constructor por movimiento
     */
    ServerThread(ServerThread &&other);
    /*
     *  Termina la comunicacion con el cliente
     */
    void stop();
    /*
     *  Retorna true is el thread ya terminó su ejecución.
     *  False sino
     */
    bool is_dead();
};

#endif
