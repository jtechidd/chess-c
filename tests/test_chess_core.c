#include <assert.h>

#include "core/chess.h"
#include "core/common.h"
#include "core/move.h"
#include "core/vector2.h"

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

    CH_Move_ParseLAN(&move, "e4e5");
    assert(move.pieceType == CH_EMPTY);
    assert(CH_Vector2_Equal(move.positionFrom, CH_Vector2_Make(4, 4)));
    assert(CH_Vector2_Equal(move.positionTo, CH_Vector2_Make(3, 4)));
    assert(move.isTaking == false);
    assert(move.promoteTo == CH_EMPTY);


    error = CH_Chess_ApplyMove(&chess, move);
    assert(error == CH_ERR_ILLEGAL_MOVE);

    return 0;
}