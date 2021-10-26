// #include "ServerThread.h"

// ServerThread::ServerThread(Proxy &proxy, QueuesMap &queuesMap) 
//                 : proxy(proxy),
//                 queuesMap(queuesMap) {}

// void ServerThread::run() {
//     std::string cmd;
//     std::string queue_name;
//     std::string message;
//     while (proxy.server_recv(cmd, queue_name, message) > 0) {
//         if (cmd == "define") {
//             this->queuesMap.define(queue_name);
//         } else if (cmd == "push") {
//             this->queuesMap.push(queue_name, message);
//         } else if (cmd == "pop") {
//             std::string msg = this->queuesMap.pop(queue_name);
//             proxy.server_send(msg);
//         } else {
//             throw -1;
//         }
//     }
// }
