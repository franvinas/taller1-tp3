#include "AcceptorThread.h"
#include <algorithm>

AcceptorThread::AcceptorThread(Socket &sk) : sk(sk) {}

void AcceptorThread::run() {
    Socket peer_sk;
    while (this->sk.accept(peer_sk) != -1) {
        this->threads.push_back(ServerThread(peer_sk, queuesMap));
        this->threads.back().start();

        for (auto it = this->threads.begin(); it != this->threads.end(); ) {
            if (it->is_dead()) {
                it->join();
                it = this->threads.erase(it);
            } else {
                ++it;
            }
        }
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
