cmake_minimum_required(VERSION 3.12)
project(snake_game)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable(${PROJECT_NAME} SnakeLogic.cpp SnakeLogic.h main.cpp)
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
