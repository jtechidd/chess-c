#include "core/move.h"
#include "core/common.h"
#include "core/piece.h"
#include "core/utils.h"

ch_error_t ch_move_parse_lan(ch_move_t *move, const char *notation) {
  size_t len;
  const char *first, *last, *cur;
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
  if ((parsed_move.piece_type = ch_char_to_piece_type(*cur)) != CH_EMPTY) {
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

static void ch_safe_write_char(char *buf, uint8_t *idx, char c,
                               uint8_t buf_len) {
  if (*idx >= buf_len - 1)
    return;
  buf[*idx] = c;
  *idx = *idx + 1;
  return;
}

void ch_move_to_str(ch_move_t *move, char *buf, uint8_t buf_len) {
  uint8_t idx = 0;
  char piece_char = ch_piece_type_to_char(move->piece_type);
  char pos_from_file = ch_horizontal_position_to_file(move->position_from.j);
  char pos_from_rank = ch_vertical_position_to_rank(move->position_from.i);
  char pos_to_file = ch_horizontal_position_to_file(move->position_to.j);
  char pos_to_rank = ch_vertical_position_to_rank(move->position_to.i);

  if (piece_char != CH_EMPTY && piece_char != 'P') {
    ch_safe_write_char(buf, &idx, piece_char, buf_len);
  }
  ch_safe_write_char(buf, &idx, pos_from_file, buf_len);
  ch_safe_write_char(buf, &idx, pos_from_rank, buf_len);
  if (move->is_taking) {
    ch_safe_write_char(buf, &idx, 'x', buf_len);
  }
  ch_safe_write_char(buf, &idx, pos_to_file, buf_len);
  ch_safe_write_char(buf, &idx, pos_to_rank, buf_len);

  buf[idx] = '\0';
}

ch_move_t ch_move_make_from_piece(ch_piece_t *piece, bool is_taking,
                                  ch_vector2_t position_to,
                                  ch_piece_type_t promote_to) {
  return (ch_move_t){
      .piece_type = piece->type,
      .position_from = piece->position,
      .is_taking = is_taking,
      .position_to = position_to,
      .promote_to = promote_to,
  };
}