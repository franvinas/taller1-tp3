#include "Socket.h"
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <utility>

/***********************
    Metodos privados
************************/

int Socket::_getaddrinfo(const char *host,
                         const char *service,
                         struct addrinfo **ptr,
                         bool passive) {
    int s = 0;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;                  /* IPv4 */
    hints.ai_socktype = SOCK_STREAM;            /* TCP  */
    hints.ai_flags = passive ? AI_PASSIVE : 0;  /* AI_PASSIVE para el server */
                                                /* y 0 para el cliente */
    s = getaddrinfo(host, service, &hints, ptr);
    if (s != 0) {
        std::cerr << "Error getaddrinfo: " << gai_strerror(s) << "\n";
        throw std::runtime_error("Error in socket getaddrinfo()");
    }
    return 0;
}

/*  Se activa la opción de reusar la direccion en caso de que esta
 *  no este disponible por un TIME_WAIT 
 */
int Socket::_reuse_address() {
    int val = 1;
    int s = setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (s == -1) throw std::runtime_error("Error in socket setsockopt()");
    return 0;
}

int Socket::_bind(struct addrinfo *ptr) {
    return bind(this->fd, ptr->ai_addr, ptr->ai_addrlen);
}

int Socket::_listen(const int &queue_length) {
    int s = listen(this->fd, queue_length);
    if (s == -1) {
        std::cerr << "Error: " << strerror(errno) << "\n";
        ::close(this->fd);
        throw std::runtime_error("Error in socket listen()");
    }
    return 0;
}

void Socket::_accept(const int &fd) {
    this->fd = fd;
}

/***********************
    Metodos publicos
************************/

Socket::Socket() : fd(-1) {}

Socket::Socket(Socket &&other) : fd(std::move(other.fd)) {
    other.fd = -1;
}

Socket::~Socket() {
    if (this->fd != -1)
        ::close(this->fd);
}

int Socket::bind_and_listen(const char *host,
                            const char *service,
                            int queue_length) {
    struct addrinfo *ptr, *rp;
    bool passive = true; // necesario para luego aceptar conexiones
    
    this->_getaddrinfo(host, service, &ptr, passive);
    for (rp = ptr; rp != NULL; rp = rp->ai_next) {
        this->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (this->fd == -1) continue;
        if (this->_bind(rp) == 0) break; // Bind exitoso
        ::close(this->fd);
    }
    freeaddrinfo(ptr);
    
    if (rp == NULL) { // Falló bind para todas las addrs
        std::cerr << "Error:" << strerror(errno) << "\n";
        ::close(this->fd);
        throw std::runtime_error("Error in socket bind()");
    }

    this->_reuse_address();
    this->_listen(queue_length);

    return 0;
}

int Socket::accept(Socket &peer) const {
    int a = ::accept(this->fd, NULL, NULL);
    if (a == -1) {
        return -1;
    }
    peer._accept(a);
    return a;
}

int Socket::connect(const char *host, const char *service) {
    struct addrinfo *ptr, *rp;
    bool passive = false; // necesario para luego usar connect
    
    this->_getaddrinfo(host, service, &ptr, passive);
    for (rp = ptr; rp != NULL; rp = rp->ai_next) {
        this->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (this->fd == -1) continue;
        if (::connect(this->fd, ptr->ai_addr, ptr->ai_addrlen) == 0) break;
        ::close(this->fd);
    }
    freeaddrinfo(ptr);
    
    if (rp == NULL) { // Falló connect para todas las addrs
        std::cerr << "Error: " << strerror(errno) << "\n";
        ::close(this->fd);
        throw std::runtime_error("Error in socket connect()");
    }

    return 0;
}

int Socket::send(const char *buffer, ssize_t len) {
    ssize_t sent_b = 0;
    while (len > sent_b) {
        ssize_t b = ::send(this->fd, buffer + sent_b, 
                            len - sent_b, MSG_NOSIGNAL);
        if (b == -1) {
            std::cerr << "Error send: " << strerror(errno) << "\n";
            throw std::runtime_error("Error in socket send()");
        } else if (b == 0) { // Socket cerrado
            return 0;
        } else {
            sent_b += b;
        }
    }
    return 0;
}

int Socket::recv(char *buffer, ssize_t len) {
    ssize_t recv_b = 0;

    while (len > recv_b) {
        ssize_t b = ::recv(this->fd, buffer + recv_b, len - recv_b, 0);
        if (b == -1) {
            std::cerr << "Error recv: " << strerror(errno) << "\n";
            throw std::runtime_error("Error in socket recv()");
        } else if (b == 0) { // Socket cerrado
            return 0;
        } else {
            recv_b += b;
        }
    }
    return recv_b;
}

void Socket::shutdown() {
    ::shutdown(this->fd, SHUT_RDWR);
}

void Socket::close() {
    ::close(this->fd);
}
