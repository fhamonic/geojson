cmake_minimum_required(VERSION 3.12)

project(GeoJSON VERSION 1.0
    DESCRIPTION "A C++17 library for parsing GeoJSON into BoostGeometry objects"
    LANGUAGES CXX)

# set(CMAKE_VERBOSE_MAKEFILE ON)

#################### Options ####################
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WARNINGS "" OFF)
option(COMPILE_FOR_NATIVE "" OFF)
option(COMPILE_WITH_LTO "" OFF)

#################### Modules ####################
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(CMakeBuildTypeUtils)

#################### Packages ####################
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

################### LIB target ####################
add_library(geojson INTERFACE)
target_include_directories(geojson INTERFACE include)
target_link_libraries(geojson PUBLIC INTERFACE CONAN_PKG::boost)
target_link_libraries(geojson PUBLIC INTERFACE CONAN_PKG::simdjson)

# ################## TEST target ####################
enable_testing()

add_executable(parsing_tests test/parsing_tests.cpp)
target_link_libraries(parsing_tests geojson)
target_link_libraries(parsing_tests CONAN_PKG::gtest)

include(GoogleTest)
gtest_discover_tests(parsing_tests)

################### EXEC target ####################
add_executable(geojson_to_svg src/geojson_to_svg.cpp)
target_include_directories(geojson_to_svg PUBLIC include)
target_link_libraries(geojson_to_svg geojson)
target_compile_options_if_exists(geojson_to_svg PRIVATE ${WARNINGS} -Wall)
target_compile_options_if_exists(geojson_to_svg PRIVATE ${COMPILE_FOR_NATIVE} -march=native)
target_compile_options_if_exists(geojson_to_svg PRIVATE ${COMPILE_WITH_LTO} -flto)