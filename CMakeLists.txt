cmake_minimum_required(VERSION 2.6)
 
add_subdirectory(gtest)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

SET(CMAKE_BUILD_TYPE Release)

add_definitions (-Wall -Wextra -std=c++11 -O3)

add_executable(main src/main.cc)
target_link_libraries(main gtest)
