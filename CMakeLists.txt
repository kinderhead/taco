cmake_minimum_required(VERSION 3.0.0)
project(taco VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.81.0 COMPONENTS iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(taco src/main.cpp src/project.cpp src/global.cpp)
include_directories(taco include)
target_include_directories(taco PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(taco PUBLIC ${Boost_LIBRARIES})
target_link_libraries(taco PRIVATE Boost::boost Boost::iostreams)

configure_file(include/config.h.in config.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
