cmake_minimum_required(VERSION 3.19)
project(boost)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_executable(boost main.cpp server.cpp session.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})