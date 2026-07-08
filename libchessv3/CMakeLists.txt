add_library(ChessV3 SHARED chess.c common.c move_db.c move.c piece_db.c piece.c vector2.c rook.c knight.c bishop.c queen.c king.c pawn.c)

add_executable(TestChessV3 test_chess_v3.c)
target_link_libraries(TestChessV3 PRIVATE ChessV3)
