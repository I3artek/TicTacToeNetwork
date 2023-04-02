//
// Created by I3artek on 02/04/2023.
//

#ifndef TICTACTOENETWORK_NETTOOLS_H
#define TICTACTOENETWORK_NETTOOLS_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

struct  socket_info
{
    struct  sockaddr_in server_addr;
    int     server_socket;
    struct  sockaddr    client_addr;
    int     client_socket;
    socklen_t client_addrlen;
};

void    print_sockname(int);
void    print_addr(struct sockaddr *);
void    print_addr_in6(struct sockaddr_in6 *);
void    print_addr_in(struct sockaddr_in *);
struct  sockaddr_in make_address(const char *, const char *);

void    create_server(struct socket_info *);
void    create_client(struct socket_info *);
void    set_address(struct socket_info *);
void    accept_client(struct socket_info *);
void    release(struct socket_info *);

struct sockaddr_in make_address(const char *address, const char *port) {
    int ret;
    struct sockaddr_in addr;
    struct addrinfo *result;
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    if ((ret = getaddrinfo(address, port, &hints, &result))) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }
    addr = *(struct sockaddr_in *) (result->ai_addr);
    freeaddrinfo(result);
    return addr;
}

void print_addr_in(struct sockaddr_in *addr) {
    char txt[128];
    const char *ret = inet_ntop(AF_INET, &addr->sin_addr, txt, sizeof(txt));
    if (ret == NULL) {
        perror("inet_ntop()");
    } else {
        printf("%s:%d", txt, htons(addr->sin_port));
    }
}

void print_addr_in6(struct sockaddr_in6 *addr) {
    char txt[128];
    const char *ret = inet_ntop(AF_INET6, &addr->sin6_addr, txt, sizeof(txt));
    if (ret == NULL) {
        perror("inet_ntop()");
    } else {
        printf("%s:%d", txt, htons(addr->sin6_port));
    }
}

void print_addr(struct sockaddr *addr) {
    switch (addr->sa_family) {
        case AF_INET:
            return print_addr_in((struct sockaddr_in *) addr);
        case AF_INET6:
            return print_addr_in6((struct sockaddr_in6 *) addr);
        default:
            printf("Unknown address family\n");
    }
}

void print_sockname(int socket) {
    struct sockaddr addr;
    socklen_t addrlen = sizeof(addr);
    if (getsockname(socket, &addr, &addrlen) < 0) {
        perror("getsockname()");
        exit(EXIT_FAILURE);
    }
    print_addr(&addr);
}


void    create_server(struct socket_info *si)
{
    set_address(si);

    si->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (si->server_socket < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (bind(si->server_socket,
             (struct sockaddr *) &si->server_addr,
             sizeof(si->server_addr)) < 0)
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(si->server_socket, 3) < 0)
    {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    printf("Server sockname: ");
    print_sockname(si->server_socket);
    printf("\n");
}

void    create_client(struct socket_info *si)
{
    set_address(si);

    si->client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (si->client_socket < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (connect(si->client_socket,
                (struct sockaddr *) &si->server_addr,
                sizeof(si->server_addr)) < 0)
    {
        perror("connect()");
        exit(EXIT_FAILURE);
    }

    printf("Connected client sockname: ");
    print_sockname(si->client_socket);
    printf("\n");
}

void    set_address(struct socket_info *si)
{
    char host[16];
    char port[6];

    printf("Type host address >");
    fflush(stdout);
    scanf("%15s", host);
    fflush(stdin);
    printf("Type port >");
    fflush(stdout);
    scanf("%5s", port);

    si->server_addr = make_address(host, port);
}

void    accept_client(struct socket_info *si)
{
    si->client_addrlen = sizeof(si->client_addr);
    si->client_socket = accept(si->server_socket,
                               &si->client_addr, &si->client_addrlen);
    if (si->client_socket < 0)
    {
        perror("accept()");
        exit(EXIT_FAILURE);
    }

    printf("Accepted client sockname: ");
    print_sockname(si->client_socket);
    printf("\n");
}

void    release(struct socket_info *si)
{
    if(si->client_socket != 0)
        close(si->client_socket);
    if(si->server_socket != 0)
        close(si->server_socket);
}

#endif //TICTACTOENETWORK_NETTOOLS_H
