#include "AcceptorThread.h"

AcceptorThread::AcceptorThread(Socket &sk) : sk(sk) {}

void AcceptorThread::run() {
    while (1) {
        Socket peer_sk;
        if (this->sk.accept(peer_sk) == -1) break;
        this->threads.push_back(ServerThread(peer_sk, queuesMap));
        this->threads.back().start();
    }
    
    for (ServerThread &t : this->threads) {
        t.stop();
        t.join();
    }
}

void AcceptorThread::stop() {
    this->sk.shutdown();
    this->sk.close();
}
