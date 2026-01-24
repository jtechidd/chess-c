cmake_minimum_required(VERSION 3.28)
project(ChessC C)

set(LIBCHESS_SOURCE_FILES
    chess.c
    board.c
    cell.c
    position.c
    move/move_array.c
    move/move.c
    pieces/piece.c
    pieces/rook.c
    pieces/knight.c
    pieces/bishop.c
    pieces/queen.c
    pieces/king.c
    pieces/pawn.c
)

add_library(chess SHARED ${LIBCHESS_SOURCE_FILES})

add_executable(main main.c)
target_link_libraries(main PRIVATE chess)