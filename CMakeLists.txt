# Root CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(utf8 VERSION 0.1.0 DESCRIPTION "ANSI C library for the 8-bit Unicode Transformation Format")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Required for clangd support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(WARN "-Wall -Wextra -Wpedantic -Werror -Wformat-security -Wshadow -fexceptions")
set(EXTRA_WARN "-Wformat -Wnull-dereference -Wdouble-promotion")
set(SANITIZE "-fsanitize=address,undefined -fno-omit-frame-pointer")
set(ANALYSIS "-Wanalyzer-double-free -Wanalyzer-file-leak -Wanalyzer-malloc-leak -Wanalyzer-null-dereference -Wanalyzer-out-of-bounds -Wanalyzer-va-list-leak")
set(COMMON "-D_FILE_OFFSET_BITS=64 ${OpenMP_C_FLAGS} ${WARN}")

set(DEBUG "${COMMON} -g3 ${EXTRA_WARN} ${SANITIZE} ${ANALYSIS}")
set(RELEASE "${COMMON} -Ofast -march=native")

find_package(OpenMP REQUIRED)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEBUG}")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${RELEASE}")
endif()

# Set the output directory for built binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add library
add_library(utf8 SHARED
    "src/test.c"
    "src/regex.c"
    "src/byte.c"
    "src/codepoint.c"
    "src/grapheme-data.c"
    "src/grapheme.c"
    "src/path.c"
)
target_include_directories(utf8 PUBLIC include)
target_link_libraries(utf8 PUBLIC m pcre2-8)

enable_testing()
add_subdirectory(tests)
add_subdirectory(examples)
