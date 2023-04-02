cmake_minimum_required(VERSION 3.20)
project(TicTacToeNetwork C)

set(CMAKE_C_STANDARD 11)

add_executable(TicTacToeNetwork main.c nettools.h tictactoe.h)
