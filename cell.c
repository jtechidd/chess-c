#include "cell.h"

cell_t cell_make(uint8_t has_piece, piece_id_t piece_id) {
  return (cell_t){has_piece, piece_id};
}
