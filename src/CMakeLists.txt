file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_library(core STATIC ${SRC_FILES})

add_executable(kramer main.c)
target_link_libraries(kramer PRIVATE core)

