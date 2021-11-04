#include "AcceptorThread.h"
#include <algorithm>

/***********************
    Metodos privados
************************/

void AcceptorThread::free_dead_threads() {
    for (auto it = this->threads.begin(); it != this->threads.end(); ) {
        if (it->is_dead()) {
            it->join();
            it = this->threads.erase(it);
        } else {
            ++it;
        }
    }
}

/***********************
    Metodos protegidos
************************/

void AcceptorThread::run() {
    Socket peer_sk;
    while (this->sk.accept(peer_sk) != -1) {
        this->threads.push_back(ServerThread(peer_sk, queuesMap));
        this->threads.back().start();

        free_dead_threads();
    }
    
    for (ServerThread &t : this->threads) {
        t.stop();
        t.join();
    }
}

/***********************
    Metodos publicos
************************/

AcceptorThread::AcceptorThread(Socket &sk) : sk(sk) {}

void AcceptorThread::stop() {
    this->sk.shutdown();
    this->sk.close();
}
