cmake_minimum_required(VERSION 3.10)
project(HuffCompressor LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "current list dir ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "cmake_build_type: ${CMAKE_BUILD_TYPE}")

get_filename_component(exe_file ${CMAKE_CURRENT_LIST_DIR} NAME)
message(STATUS "exe file name: ${exe_file}")

file(GLOB src_files ${CMAKE_SOURCE_DIR}/src/*.[ch]pp)

add_subdirectory(utils)

add_compile_options(-Wall -O2 )


if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("Debug mode")
    add_compile_options(-g -ggdb)
endif()


add_executable(HuffCompressor main.cpp ${src_files})


find_package(spdlog REQUIRED)
target_include_directories(${exe_file} PUBLIC  GoogleTest)

target_link_libraries(${exe_file} PUBLIC gtest utils PRIVATE spdlog::spdlog)
target_link_directories(${exe_file} PUBLIC /usr/local/lib)

target_include_directories(HuffCompressor PUBLIC   ${CMAKE_SOURCE_DIR}/include)


