#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include "Thread.h"
#include "ServerProtocol.h"
#include "QueuesMap.h"
#include <atomic>
#include <string>

/*
 *  ServerThread hereda de Thread e implementa run().
 *  Todas las instancias ServerThread tienen una referencia a la queuesMap
 *  ya que este es un recurso compartido
 */
class ServerThread: public Thread {
private:
    ServerProtocol protocol;    
    QueuesMap &queuesMap;
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_running;
    void exec_cmd(const std::string &cmd, 
                  const std::string &queue_name, 
                  const std::string &message);
    
protected:
    /*
     *  La ejecucion de un hilo ServerThread consiste en recibir comandos
     *  del cliente y ejecutarlos.
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
