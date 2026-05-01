cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed."  
                        "Please run CMake from a separate build directory.")
endif()

project(mf LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

add_executable(mf)

target_sources(mf PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core.cpp
)

target_compile_features(mf PRIVATE cxx_std_20)
target_compile_options(mf PRIVATE -Wall -Wextra -g)

target_link_libraries(mf PRIVATE PkgConfig::GTKMM)

