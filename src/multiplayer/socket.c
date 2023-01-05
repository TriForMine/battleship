#include "socket.h"

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

int socket_create(int domain, int type, int protocol) {
#ifdef _WIN32
    WSADATA wsa_data;
  int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (result != 0) {
    return -1;
  }
#endif

    return socket(domain, type, protocol);
}

int socket_create_nonblocking(int domain, int type, int protocol) {
    int sockfd = socket_create(domain, type, protocol);
#ifdef _WIN32
    unsigned long mode = 1;
#else
    int flags = fcntl(sockfd, F_GETFL, 0);
#endif
    if (sockfd < 0) {
        return sockfd;
    }

#ifdef _WIN32
    if (ioctlsocket(sockfd, FIONBIO, &mode) != 0) {
      socket_close(sockfd);
      return -1;
    }
#else
    if (flags < 0 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        socket_close(sockfd);
        return -1;
    }
#endif

    return sockfd;
}

int socket_bind(int sockfd, const sockaddr_in *addr) {
    return bind(sockfd, (const sockaddr *) addr, sizeof(sockaddr_in));
}

int socket_listen(int sockfd, int backlog) {
    return listen(sockfd, backlog);
}

int socket_accept(int sockfd, sockaddr_in *addr) {
    socklen_t addrlen = sizeof(sockaddr_in);
    return accept(sockfd, (sockaddr *) addr, &addrlen);
}

int socket_connect(int sockfd, const sockaddr_in *addr) {
    return connect(sockfd, (const sockaddr *) addr, sizeof(sockaddr_in));
}

int socket_send(int sockfd, const void *buf, size_t len, int flags) {
    return send(sockfd, buf, len, flags);
}

int socket_recv(int sockfd, void *buf, size_t len, int flags) {
    return recv(sockfd, buf, len, flags);
}


int socket_close(int sockfd) {
#ifdef _WIN32
    return closesocket(sockfd);
#else
    return close(sockfd);
#endif
}