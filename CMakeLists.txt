cmake_minimum_required(VERSION 3.10)
project(chess VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE CHESS_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

find_package(PkgConfig REQUIRED)
find_package(fmt REQUIRED)

pkg_check_modules(OPENAL REQUIRED openal)
pkg_check_modules(SNDFILE REQUIRED sndfile)

add_executable(chess ${CHESS_SOURCES})

target_include_directories(chess PRIVATE
    ${OPENAL_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

find_library(OPENAL_FRAMEWORK OpenAL)

target_link_libraries(chess PRIVATE
    fmt::fmt
    ${OPENAL_LDFLAGS}
    ${SNDFILE_LDFLAGS}
)

target_compile_options(chess PRIVATE
    -march=native
)
