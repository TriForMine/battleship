#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#include <io.h>
#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#endif

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int socket_create(int domain, int type, int protocol);

int socket_create_nonblocking(int domain, int type, int protocol);

int socket_bind(int sockfd, const sockaddr_in *addr);

int socket_listen(int sockfd, int backlog);

int socket_accept(int sockfd, sockaddr_in *addr);

int socket_connect(int sockfd, const sockaddr_in *addr);

int socket_send(int sockfd, const void *buf, size_t len, int flags);

int socket_recv(int sockfd, void *buf, size_t len, int flags);

int socket_close(int sockfd);

#endif