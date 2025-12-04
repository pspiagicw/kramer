cmake_minimum_required(VERSION 3.10)
project(kramer C)

set(CMAKE_C_STANDARD 11)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CTEST_OUTPUT_ON_FAILURE ON)

# Include headers globally
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add src subdirectories
add_subdirectory(src)

# Enable testing (CTest)
enable_testing()
add_subdirectory(tests)

