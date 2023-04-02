/**
 * \brief Basic TCP example - intraprocess communication from client to server.
 */

#include "nettools.h"

#define MSG_SIZE 20


int main(int argc, const char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "USAGE: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in this_addr = make_address(argv[1], argv[2]);

    // Server socket configuration

    int this_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this_socket < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    if (bind(this_socket, (struct sockaddr *) &this_addr, sizeof(this_addr)) < 0) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(this_socket, 3) < 0) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    char other_host[16];
    char other_port[6];

    printf("Your sockname: ");
    print_sockname(this_socket);
    printf("\n");

    // Client socket creation and establishing connection

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    printf("Type other player's adress >");
    fflush(stdout);
    scanf("%15s", other_host);
    fflush(stdin);
    printf("Type other player's port >");
    fflush(stdout);
    scanf("%5s", other_port);

    struct sockaddr_in other_addr = make_address(other_host, other_port);

    printf("connect? >");
    fflush(stdout);
    getchar();
    getchar();

    if (connect(client_socket, (struct sockaddr *) &other_addr, sizeof(other_addr)) < 0) {
        perror("connect()");
        exit(EXIT_FAILURE);
    }

//    printf("Connected client sockname: ");
//    print_sockname(client_socket);
//    printf("\n");

    printf("accept? >");
    fflush(stdout);
    getchar();

    socklen_t other_addrlen = sizeof(other_addr);
    int accepted_other_socket = accept(this_socket, &other_addr, &other_addrlen);
    if (accepted_other_socket < 0) {
        perror("accept()");
        exit(EXIT_FAILURE);
    }

    printf("Accepted other player sockname: ");
    print_sockname(accepted_other_socket);
    printf("\n");

    printf("send? >");
    fflush(stdout);
    getchar();

    int what_next = 0;

    char msg[MSG_SIZE + 1];
    ssize_t ret;

    do {
        printf("1. Send Message\n");
        printf("2. Receive Message\n");
        printf("3. Exit\n");
        scanf("%d", &what_next);
        switch (what_next) {
            case 1:
                printf("Input your message >");
                fflush(stdout);
                scanf("%20s", msg);
                printf("Send message \"%s\"? >", msg);
                getchar();
                if (send(client_socket, msg, MSG_SIZE, 0) < 0) {
                    perror("sendto()");
                    exit(EXIT_FAILURE);
                }
                break;
            case 2:
                printf("Receiving the message...\n");
                ret = recv(accepted_other_socket, msg, MSG_SIZE, 0);
                if (ret < 0) {
                    perror("sendto()");
                    exit(EXIT_FAILURE);
                }
                printf("Received message:\n");
                printf("%s\n", msg);
                break;
        }
    } while (what_next != 3);

   // printf("Received data of size %ld bytes: '%s'\n", ret, buff);

    printf("close client? >");
    fflush(stdout);
    getchar();

    close(client_socket);

    printf("close server? >");
    fflush(stdout);
    getchar();

    close(accepted_other_socket);
    close(this_socket);

    return 0;
}
