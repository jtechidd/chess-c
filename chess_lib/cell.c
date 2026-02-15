#include "cell.h"

cell_t cell_make(bool has_piece, piece_id_t piece_id) {
  return (cell_t){has_piece, piece_id};
}
