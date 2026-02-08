#ifndef PIECE_MACROS_H
#define PIECE_MACROS_H


#define DEFINE_PIECE_NEW_INTERNAL(piece_type, piece_type_capitalized)                                                  \
  int _##piece_type##_new(piece_type##_t **piece_type##_out, piece_id_t piece_id, side_t side, vector2_t position) {   \
    if (!(piece_type##_out && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) { \
      return CHESS_ERROR_INVALID_ARGS;                                                                                 \
    }                                                                                                                  \
    piece_type##_t *piece_type = (piece_type##_t *)malloc(sizeof(piece_type##_t));                                     \
    if (!piece_type) {                                                                                                 \
      return CHESS_ERROR_NO_MEMORY;                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    piece_type->piece.id = piece_id;                                                                                   \
    piece_type->piece.side = side;                                                                                     \
    piece_type->piece.type = PIECE_TYPE_##piece_type_capitalized;                                                      \
    piece_type->piece.position = position;                                                                             \
    piece_type->piece.is_captured = 0;                                                                                 \
    piece_type->piece.moving_count = 0;                                                                                \
                                                                                                                       \
    piece_type->piece.piece_clone = piece_type##_piece_clone;                                                          \
    piece_type->piece.piece_free = piece_type##_piece_free;                                                            \
    piece_type->piece.piece_get_moves = piece_type##_piece_get_moves;                                                  \
                                                                                                                       \
    *piece_type##_out = piece_type;                                                                                    \
    return CHESS_OK;                                                                                                   \
  }

#define DEFINE_PIECE_CLONE_INTERNAL(piece_type)                                                                                                             \
  int _##piece_type##_clone(piece_type##_t **piece_type##_out, piece_type##_t *piece_type##_src) {                                                          \
    if (!(piece_type##_out && piece_type##_src)) {                                                                                                          \
      return CHESS_ERROR_INVALID_ARGS;                                                                                                                      \
    }                                                                                                                                                       \
    int err;                                                                                                                                                \
    piece_type##_t *piece_type;                                                                                                                             \
    if ((err = _##piece_type##_new(&piece_type, piece_type##_src->piece.id, piece_type##_src->piece.side, piece_type##_src->piece.position)) != CHESS_OK) { \
      return err;                                                                                                                                           \
    }                                                                                                                                                       \
                                                                                                                                                            \
    piece_type->piece.is_captured = piece_type##_src->piece.is_captured;                                                                                    \
    piece_type->piece.moving_count = piece_type##_src->piece.moving_count;                                                                                  \
    *piece_type##_out = piece_type;                                                                                                                         \
    return CHESS_OK;                                                                                                                                        \
  }

#define DEFINE_PIECE_FREE_INTERNAL(piece_type)           \
  int _##piece_type##_free(piece_type##_t *piece_type) { \
    if (!piece_type) {                                   \
      return CHESS_OK;                                   \
    }                                                    \
    free(piece_type);                                    \
    return CHESS_OK;                                     \
  }

#define DEFINE_PIECE_GET_MOVES_INTERNAL(function_name, piece_type, piece_type_capitalized)                  \
  int function_name(move_array_t **move_array_out, piece_type##_t *piece_type, board_t *board) {            \
    if (!(move_array_out && piece_type && board)) {                                                         \
      return CHESS_ERROR_INVALID_ARGS;                                                                      \
    }                                                                                                       \
    int err;                                                                                                \
    move_array_t *move_array = move_array_new();                                                            \
    for (size_t k = 0; k < piece_type_capitalized##_TOTAL_DIRECTIONS; k++) {                                \
      vector2_t direction = piece_type_capitalized##_DIRECTIONS[k];                                         \
      for (int scale = 1; scale <= piece_type_capitalized##_MAX_SCALE; scale++) {                           \
        vector2_t position_to = vector2_add2(piece_type->piece.position, vector2_scaled(direction, scale)); \
        if (!is_position_in_bound(position_to)) {                                                           \
          break;                                                                                            \
        }                                                                                                   \
        bool has_piece_on_position;                                                                         \
        if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {  \
          goto fail;                                                                                        \
        }                                                                                                   \
        if (!has_piece_on_position) {                                                                       \
          move_array_add(move_array, move_new_moving_piece(piece_type->piece.id, position_to));             \
          continue;                                                                                         \
        }                                                                                                   \
        piece_t *piece;                                                                                     \
        if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {                  \
          goto fail;                                                                                        \
        }                                                                                                   \
        if (piece_is_opposite(&piece_type->piece, piece)) {                                                 \
          move_array_add(move_array, move_new_taking_piece(piece_type->piece.id, position_to, piece->id));  \
        }                                                                                                   \
        break;                                                                                              \
      }                                                                                                     \
    }                                                                                                       \
    *move_array_out = move_array;                                                                           \
    return CHESS_OK;                                                                                        \
  fail:                                                                                                     \
    if (move_array != NULL) {                                                                               \
      move_array_free(move_array);                                                                          \
      move_array = NULL;                                                                                    \
    }                                                                                                       \
    return err;                                                                                             \
  }

#define DEFINE_PIECE_NEW(piece_type)                                                                          \
  int piece_type##_piece_new(piece_t **piece_t, piece_id_t piece_id, side_t side, vector2_t position) {       \
    if (!(piece_t && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) { \
      return CHESS_ERROR_INVALID_ARGS;                                                                        \
    }                                                                                                         \
    int err;                                                                                                  \
    piece_type##_t *piece_type;                                                                               \
    if ((err = _##piece_type##_new(&piece_type, piece_id, side, position)) != CHESS_OK) {                     \
      return err;                                                                                             \
    }                                                                                                         \
    *piece_t = &piece_type->piece;                                                                            \
    return CHESS_OK;                                                                                          \
  }

#define DEFINE_PIECE_CAST(piece_type, piece_type_capitalized)                          \
  int piece_type##_piece_cast(piece_type##_t **piece_type##_out, piece_t *piece) {     \
    if (!(piece_type##_out && piece)) {                                                \
      return CHESS_ERROR_INVALID_ARGS;                                                 \
    }                                                                                  \
    if (piece->type == PIECE_TYPE_##piece_type_capitalized) {                          \
      *piece_type##_out = (piece_type##_t *)(piece - offsetof(piece_type##_t, piece)); \
      return CHESS_OK;                                                                 \
    }                                                                                  \
    return CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH;                                       \
  }

#define DEFINE_PIECE_CLONE(piece_type)                                                 \
  int piece_type##_piece_clone(piece_t **piece_type##_piece_out, piece_t *piece) {     \
    if (!(piece_type##_piece_out && piece)) {                                          \
      return CHESS_ERROR_INVALID_ARGS;                                                 \
    }                                                                                  \
    int err;                                                                           \
    piece_type##_t *piece_type, *cloned_##piece_type;                                  \
    if ((err = piece_type##_piece_cast(&piece_type, piece)) != CHESS_OK) {             \
      return err;                                                                      \
    }                                                                                  \
    if ((err = _##piece_type##_clone(&cloned_##piece_type, piece_type)) != CHESS_OK) { \
      return err;                                                                      \
    }                                                                                  \
    *piece_type##_piece_out = &cloned_##piece_type->piece;                             \
    return CHESS_OK;                                                                   \
  }

#define DEFINE_PIECE_GET_MOVES(piece_type)                                                          \
  int piece_type##_piece_get_moves(move_array_t **move_array_out, piece_t *piece, board_t *board) { \
    if (!(move_array_out && piece && board)) {                                                      \
      return CHESS_ERROR_INVALID_ARGS;                                                              \
    }                                                                                               \
    int err;                                                                                        \
    piece_type##_t *piece_type;                                                                     \
    if ((err = piece_type##_piece_cast(&piece_type, piece)) != CHESS_OK) {                          \
      return err;                                                                                   \
    }                                                                                               \
    if ((err = _##piece_type##_get_moves(move_array_out, piece_type, board)) != CHESS_OK) {         \
      return err;                                                                                   \
    }                                                                                               \
    return CHESS_OK;                                                                                \
  }

#define DEFINE_PIECE_FREE(piece_type)                                      \
  int piece_type##_piece_free(piece_t *piece) {                            \
    if (!piece) {                                                          \
      return CHESS_OK;                                                     \
    }                                                                      \
    int err;                                                               \
    piece_type##_t *piece_type;                                            \
    if ((err = piece_type##_piece_cast(&piece_type, piece)) != CHESS_OK) { \
      return err;                                                          \
    }                                                                      \
    if ((err = _##piece_type##_free(piece_type)) != CHESS_OK) {            \
      return err;                                                          \
    }                                                                      \
    return CHESS_OK;                                                       \
  }

#define DEFINE_BOARD_IS_POSITION_GET_ATTACKED_BY_PIECE(piece_type, piece_type_capitalized)                              \
  int board_is_position_get_attacked_by_##piece_type(bool *bool_out, board_t *board, side_t side, vector2_t position) { \
    if (!(board && is_side_valid(side) && is_position_in_bound(position))) {                                            \
      return CHESS_ERROR_INVALID_ARGS;                                                                                  \
    }                                                                                                                   \
    int err;                                                                                                            \
    for (size_t k = 0; k < piece_type_capitalized##_TOTAL_DIRECTIONS; k++) {                                            \
      vector2_t direction = piece_type_capitalized##_DIRECTIONS[k];                                                     \
      for (int scale = 1; scale <= piece_type_capitalized##_MAX_SCALE; scale++) {                                       \
        vector2_t position_to = vector2_add2(position, vector2_scaled(direction, scale));                               \
        if (!is_position_in_bound(position_to)) {                                                                       \
          break;                                                                                                        \
        }                                                                                                               \
        bool has_piece_on_position;                                                                                     \
        if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {              \
          return err;                                                                                                   \
        }                                                                                                               \
        if (!has_piece_on_position) {                                                                                   \
          continue;                                                                                                     \
        }                                                                                                               \
        piece_t *piece;                                                                                                 \
        if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {                              \
          return err;                                                                                                   \
        }                                                                                                               \
        piece_type##_t *piece_type;                                                                                     \
        if ((err = piece_type##_piece_cast(&piece_type, piece)) != CHESS_OK) {                                          \
          if (err == CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH) {                                                            \
            break;                                                                                                      \
          } else {                                                                                                      \
            return err;                                                                                                 \
          }                                                                                                             \
        }                                                                                                               \
        if (is_opposite_side(side, piece_type->piece.side)) {                                                           \
          *bool_out = true;                                                                                             \
          return CHESS_OK;                                                                                              \
        }                                                                                                               \
      }                                                                                                                 \
    }                                                                                                                   \
    *bool_out = false;                                                                                                  \
    return CHESS_OK;                                                                                                    \
  }

#endif