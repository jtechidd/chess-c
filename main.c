#include "board.h"
#include "chess.h"

int main(int argc, char* argv[]) {
    Chess* chess = Chess_New();

    Board_Debug(chess->board);
}