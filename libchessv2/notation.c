#include "notation.h"
#include "piece_db.h"

#include <stdio.h>
#include <string.h>

inline char get_file_from_position(vector2_t *position) {
  return 'a' + position->j;
}

inline char get_rank_from_position(vector2_t *position) {
  return '0' + BOARD_HEIGHT - position->i;
}

void notation_init_from_move(notation_t *notation, piece_db_t *piece_db,
                             move_t *move) {
  memset(notation, 0, sizeof(notation_t));

  notation->type = NOTATION_TYPE_MOVE;

  char *data_cur = notation->data;
  char *data_end = notation->data + NOTATION_DATA_MAX_SIZE - 1;

  piece_t *piece = piece_db_get_by_id(piece_db, move->piece_id);

  char piece_symbol = EMPTY;
  switch (piece->type) {
  case PIECE_TYPE_ROOK:
    piece_symbol = 'R';
    break;
  case PIECE_TYPE_KNIGHT:
    piece_symbol = 'N';
    break;
  case PIECE_TYPE_BISHOP:
    piece_symbol = 'B';
    break;
  case PIECE_TYPE_QUEEN:
    piece_symbol = 'Q';
    break;
  case PIECE_TYPE_KING:
    piece_symbol = 'K';
    break;
  }

  if (data_cur < data_end && piece_symbol != EMPTY)
    *data_cur++ = piece_symbol;
  if (data_cur < data_end)
    *data_cur++ = get_file_from_position(&move->position_from);
  if (data_cur < data_end)
    *data_cur++ = get_rank_from_position(&move->position_from);

  if (data_cur < data_end) {
    if (move->flags & MOVE_FLAGS_TAKE_PIECE) {
      *data_cur++ = 'x';
    } else if (move->flags & MOVE_FLAGS_MOVE_PIECE) {
      *data_cur++ = '-';
    }
  }

  if (data_cur < data_end)
    *data_cur++ = get_file_from_position(&move->position_to);
  if (data_cur < data_end)
    *data_cur++ = get_rank_from_position(&move->position_to);
}

void notation_init_from_position(notation_t *notation, vector2_t *position) {
  memset(notation, 0, sizeof(notation_t));

  notation->type = NOTATION_TYPE_POSITION;

  notation->data[0] = get_file_from_position(position);
  notation->data[1] = get_rank_from_position(position);
}