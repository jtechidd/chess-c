#include "core/utils.h"
#include "core/common.h"

bool ch_is_valid_file(char file) {
  return isalpha(file) && file >= 'a' && file <= 'h';
}

bool ch_is_valid_rank(char rank) {
  return isdigit(rank) && rank >= '1' && rank <= '8';
}

int8_t ch_rank_to_vertical_position(char rank) {
  if (!ch_is_valid_rank(rank)) {
    return -1;
  }
  return CH_BOARD_HEIGHT - (rank - '0');
}

int8_t ch_file_to_horizontal_position(char file) {
  if (!ch_is_valid_file(file)) {
    return -1;
  }
  return file - 'a';
}

ch_piece_type_t ch_char_to_piece_type(char piece_char) {
  switch (piece_char) {
  case 'R':
    return CH_PIECE_TYPE_ROOK;
  case 'N':
    return CH_PIECE_TYPE_KNIGHT;
  case 'B':
    return CH_PIECE_TYPE_BISHOP;
  case 'Q':
    return CH_PIECE_TYPE_QUEEN;
  case 'K':
    return CH_PIECE_TYPE_KING;
  case 'P':
    return CH_PIECE_TYPE_PAWN;
  }
  return CH_EMPTY;
}

char ch_piece_type_to_char(ch_piece_type_t type) {
  switch (type) {
  case CH_PIECE_TYPE_ROOK:
    return 'R';
  case CH_PIECE_TYPE_KNIGHT:
    return 'N';
  case CH_PIECE_TYPE_BISHOP:
    return 'B';
  case CH_PIECE_TYPE_QUEEN:
    return 'Q';
  case CH_PIECE_TYPE_KING:
    return 'K';
  case CH_PIECE_TYPE_PAWN:
    return 'P';
  }
  return CH_EMPTY;
}

bool ch_is_position_in_bound(ch_vector2_t position) {
  return position.i >= 0 && position.i < CH_BOARD_HEIGHT && position.j >= 0 &&
         position.j < CH_BOARD_WIDTH;
}
