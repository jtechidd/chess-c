#include "core/move.h"
#include "core/utils.h"

ch_error_t ch_move_parse_lan(ch_move_t *move, const char *notation) {
  size_t len;
  const char *first, *last, *cur;
  ch_piece_type_t piece_type;
  uint8_t v_pos_from, h_pos_from, v_pos_to, h_pos_to;
  ch_move_t parsed_move;

  memset(&parsed_move, 0, sizeof(ch_move_t));

  len = strlen(notation);
  if (len < 2) {
    return CH_ERR_INVALID_NOTATION;
  }

  first = notation;
  last = notation + len - 1;
  cur = first;

  // 1. Parse piece type
  if ((piece_type = ch_char_to_piece_type(*cur)) != CH_EMPTY) {
    cur++;
  }

  if (cur > last) {
    return CH_ERR_INVALID_NOTATION;
  }

  // 2. Parse position from
  if (cur <= last && ch_is_valid_file(*cur)) {
    h_pos_from = ch_file_to_horizontal_position(*cur);
    cur++;
  } else {
    return CH_ERR_INVALID_NOTATION;
  }

  if (cur <= last && ch_is_valid_rank(*cur)) {
    v_pos_from = ch_rank_to_vertical_position(*cur);
    cur++;
  } else {
    return CH_ERR_INVALID_NOTATION;
  }

  parsed_move.position_from = ch_vector2_make(v_pos_from, h_pos_from);

  // 3. Parse action
  if (cur > last) {
    return CH_ERR_INVALID_NOTATION;
  }

  if (*cur == 'x') {
    parsed_move.is_taking = true;
    cur++;
  } else {
    parsed_move.is_taking = false;
  }

  // 4. Parse position to
  if (cur <= last && ch_is_valid_file(*cur)) {
    h_pos_to = ch_file_to_horizontal_position(*cur);
    cur++;
  } else {
    return CH_ERR_INVALID_NOTATION;
  }

  if (cur <= last && ch_is_valid_rank(*cur)) {
    v_pos_to = ch_rank_to_vertical_position(*cur);
    cur++;
  } else {
    return CH_ERR_INVALID_NOTATION;
  }

  parsed_move.position_to = ch_vector2_make(v_pos_to, h_pos_to);

  // 5. Parse promotion
  if (cur <= last &&
      (parsed_move.promote_to = ch_char_to_piece_type(*cur)) != CH_EMPTY) {
    cur++;
  }

  memcpy(move, &parsed_move, sizeof(ch_move_t));

  return CH_ERR_SUCCESS;
}
