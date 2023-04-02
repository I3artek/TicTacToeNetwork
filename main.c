//
// Created by I3artek on 02/04/2023.
//

#include    "nettools.h"
#include    "tictactoe.h"

#define MSG_SIZE 20

int main(int argc, const char *argv[])
{
    struct game g = {0};
    int player_id;

    struct socket_info si = {};

    printf("Hello\n");
    printf("1. Host Game\n");
    printf("2. Join Game\n");
    scanf("%d", &player_id);
    switch (player_id) {
        case 1:
            create_server(&si);
            accept_client(&si);
            break;
        case 2:
            create_client(&si);
            break;
    }

    game_loop(&g, player_id, &si);

//    char msg[MSG_SIZE + 1];
//    ssize_t ret;
//
//    do {
//        printf("1. Send Message\n");
//        printf("2. Receive Message\n");
//        printf("3. Exit\n");
//        scanf("%d", &what_next);
//        switch (what_next) {
//            case 1:
//                printf("Input your message >");
//                fflush(stdout);
//                scanf("%20s", msg);
//                if (send(si.client_socket, msg, MSG_SIZE, 0) < 0) {
//                    perror("sendto()");
//                    exit(EXIT_FAILURE);
//                }
//                break;
//            case 2:
//                printf("Receiving the message...\n");
//                ret = recv(si.client_socket, msg, MSG_SIZE, 0);
//                if (ret < 0) {
//                    perror("sendto()");
//                    exit(EXIT_FAILURE);
//                }
//                printf("Received message:\n");
//                printf("%s\n", msg);
//                break;
//        }
//    } while (what_next != 3);

    release(&si);

    return 0;
}

