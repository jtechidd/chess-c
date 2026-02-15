set(CHESS_LIB_SOURCE_FILES
    chess.c
    board.c
    cell.c
    vector2.c
    utils.c
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

add_library(chess SHARED ${CHESS_LIB_SOURCE_FILES})

target_include_directories(chess PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})