cmake_minimum_required(VERSION 3.5)

project(events-handling2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(events-handling2 main.cpp)

install(TARGETS events-handling2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
