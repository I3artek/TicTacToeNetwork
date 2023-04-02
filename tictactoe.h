//
// Created by I3artek on 03/04/2023.
//

#ifndef TICTACTOENETWORK_TICTACTOE_H
#define TICTACTOENETWORK_TICTACTOE_H

#include "nettools.h"

struct  game
{
    /* 0 - free, 1 - X, 2 - O */
    int board[9];
};

char    get_symbol(int);
void    print_board(struct game *);
void    game_loop(struct game *, int, struct socket_info *);
void    make_move(struct game *, int);
int     game_should_end(struct game *);
void    send_game_info(struct game *, struct socket_info *);
void    receive_game_info(struct game *, struct socket_info *);

char    get_symbol(int n)
{
    switch (n) {
        case 1:
            return 'X';
        case 2:
            return 'O';
        default:
            return '-';
    }
}

void    print_board(struct game *g)
{
    printf("%c %c %c \n",
           get_symbol(g->board[0]),
           get_symbol(g->board[1]),
           get_symbol(g->board[2]));
    printf("%c %c %c \n",
           get_symbol(g->board[3]),
           get_symbol(g->board[4]),
           get_symbol(g->board[5]));
    printf("%c %c %c \n",
           get_symbol(g->board[6]),
           get_symbol(g->board[7]),
           get_symbol(g->board[8]));
    printf("\n");
}

void    game_loop(struct game *g, int player_id, struct socket_info *si)
{
    if(player_id == 1)
    {
        make_move(g, player_id);
        send_game_info(g, si);
    }
    while (!game_should_end(g))
    {
        printf("Receving input from other player...\n");
        receive_game_info(g, si);
        printf("Time for your input!\n");
        make_move(g, player_id);
        send_game_info(g, si);
    }
}

void    send_game_info(struct game *g, struct socket_info *si)
{
    int buf[9];
    for(int i = 0; i < 9; i++)
    {
        buf[i] = g->board[i];
    }
    if (send(si->client_socket, buf, sizeof(buf), 0) < 0) {
        perror("sendto()");
        exit(EXIT_FAILURE);
    }
}

void receive_game_info(struct game *g, struct socket_info *si)
{
    int buf[9];

    ssize_t ret = recv(si->client_socket, buf, sizeof(buf), 0);
    if (ret < 0) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < 9; i++)
    {
        g->board[i] = buf[i];
    }
}

void    print_instructions(struct game *g, int player_id)
{

}

void    make_move(struct game *g, int player_id)
{
    int file = -1;
    while(file < 0 || file > 8 || g->board[file] != 0)
    {
        print_board(g);
        printf("Choose your file >");
        fflush(stdout);
        scanf("%d", &file);
    }
    g->board[file] = player_id;
    print_board(g);
}

/* returns id of player who won or 0 */
//todo: Check only files that were affected by last move
int     game_should_end(struct game *g)
{
    /*
     * if sum = 6, player 2 wins
     * if sum = 3, player 1 wins
     * otherwise, the game continues
     */
    //todo: whole function
    return 0;
}

#endif //TICTACTOENETWORK_TICTACTOE_H
