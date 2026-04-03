#include "chess.h"
#include "piece.h"
#include "vector2.h"

#define NUM_DIRECTIONS 4
static const vector2_t directions[NUM_DIRECTIONS] = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}};

void piece_data_init_rook(piece_data_t *piece_data, uint8_t type) {
  piece_data_init_empty(piece_data);
  piece_data->rook.type = type;
}

static void piece_fill_moves_rook(chess_t *chess, piece_t *piece) {
  board_t *board = &chess->board;

  vector2_t position_target, direction_scaled;
  move_t move;
  piece_t *piece_target;

  for (uint8_t k = 0; k < NUM_DIRECTIONS; k++) {
    for (uint8_t s = 1; s < 8; s++) {
      vector2_scaled(&direction_scaled, (vector2_t *)&directions[k], s);
      vector2_add(&position_target, &piece->position,
                  (vector2_t *)&directions[k]);
      if (!is_position_in_bound(&position_target)) {
        continue;
      }
      if (board_is_position_empty(board, &position_target)) {
        move_init_move_piece(&move, piece, &position_target);
        chess_add_legal_move(chess, &move);
        continue;
      }
      piece_target = board_get_piece_on_position(board, &position_target);
      if (piece_is_opposite(piece, piece_target)) {
        move_init_take_piece(&move, piece, &position_target, piece_target);
        chess_add_legal_move(chess, &move);
      }
      break;
    }
  }
}

const piece_methods_t PIECE_METHODS_ROOK = {
    .piece_fill_moves = piece_fill_moves_rook,
};
