file(GLOB logging_srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)

add_library(logging ${logging_srcs})

target_include_directories(logging
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)