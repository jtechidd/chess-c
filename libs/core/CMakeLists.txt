file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(ChessCore SHARED ${SOURCES})

target_include_directories(ChessCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
