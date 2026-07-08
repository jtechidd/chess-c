#include <assert.h>

#include "chess.h"
#include "common.h"
#include "move.h"
#include "vector2.h"

int main(int argc, char* argv[]) {
    CH_Chess chess;
    CH_Move move;
    CH_Error error;

    CH_Chess_InitStandard(&chess);

    CH_Move_ParseLAN(&move, "e2e4");
    assert(move.pieceType == CH_EMPTY);
    assert(CH_Vector2_Equal(move.positionFrom, CH_Vector2_Make(6, 4)));
    assert(CH_Vector2_Equal(move.positionTo, CH_Vector2_Make(4, 4)));
    assert(move.isTaking == false);
    assert(move.promoteTo == CH_EMPTY);

    error = CH_Chess_ApplyMove(&chess, move);
    assert(error == CH_ERR_SUCCESS);

    CH_Move_ParseLAN(&move, "e7e5");
    assert(move.pieceType == CH_EMPTY);
    assert(CH_Vector2_Equal(move.positionFrom, CH_Vector2_Make(1, 4)));
    assert(CH_Vector2_Equal(move.positionTo, CH_Vector2_Make(3, 4)));
    assert(move.isTaking == false);
    assert(move.promoteTo == CH_EMPTY);

    error = CH_Chess_ApplyMove(&chess, move);
    assert(error == CH_ERR_SUCCESS);


    return 0;
}