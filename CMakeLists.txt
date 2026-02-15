cmake_minimum_required(VERSION 3.28)
project(ChessC C)

add_subdirectory(chess_lib)

add_executable(chess_slate slate.c)
target_link_libraries(chess_slate PRIVATE chess)