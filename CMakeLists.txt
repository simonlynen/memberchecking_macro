cmake_minimum_required(VERSION 2.6)
 
add_subdirectory(gtest)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_definitions (-Wall -Wextra -pedantic -std=c++11)

add_executable(main src/main.cc)
target_link_libraries(main gtest)
