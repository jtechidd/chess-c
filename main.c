#include "board.h"
#include "chess.h"

int main(int argc, char* argv[]) {
    Chess* chess = Chess_New();

    board_debug(chess->board);

    MoveArray *moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
}