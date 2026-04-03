set(CHESS_LIB_SOURCE_FILES
  chess.c
  piece.c
  piece_db.c
  move_db.c
  move.c
  vector2.c
  rook.c
  knight.c
  bishop.c
  queen.c
  king.c
  pawn.c
)

add_library(ChessV2 SHARED ${CHESS_LIB_SOURCE_FILES})
target_compile_options(ChessV2 PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(ChessV2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(TestChessV2 test_chess.c)
target_link_libraries(TestChessV2 PRIVATE ChessV2)
