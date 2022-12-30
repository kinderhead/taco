cmake_minimum_required(VERSION 3.0.0)
project(taco VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(taco src/main.cpp src/project.cpp)
include_directories(taco include)
target_include_directories(taco PUBLIC "${PROJECT_BINARY_DIR}")

configure_file(include/config.h.in config.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
