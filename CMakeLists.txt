cmake_minimum_required(VERSION 3.16)

project(
  chess
  VERSION 0.0.1
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add LTO for optimisation
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

file(GLOB_RECURSE CHESS_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_SOURCE_DIR}/src/*.c")

find_package(PkgConfig REQUIRED)
find_package(fmt REQUIRED)
find_package(SFML REQUIRED COMPONENTS System Window Graphics Audio)

pkg_check_modules(OPENAL REQUIRED openal)
pkg_check_modules(SNDFILE REQUIRED sndfile)

add_executable(chess ${CHESS_SOURCES})

target_include_directories(
  chess PRIVATE ${CMAKE_SOURCE_DIR}/include ${OPENAL_INCLUDE_DIRS}
                ${SNDFILE_INCLUDE_DIRS})

find_library(OPENAL_FRAMEWORK OpenAL)

target_link_libraries(chess PRIVATE fmt::fmt ${OPENAL_LDFLAGS}
                                    ${SNDFILE_LDFLAGS})

target_link_libraries(chess PRIVATE SFML::System SFML::Window SFML::Graphics
                                    SFML::Audio)

target_compile_options(chess PRIVATE -O3 -march=native -mbmi2
                                     -fomit-frame-pointer)

if(LLD_LINKER)
  target_link_options(chess PRIVATE -fuse-ld=lld)
endif()

target_link_options(chess PRIVATE -Wl,--as-needed -Wl,--gc-sections)
