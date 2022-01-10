cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED yes)

Project(CEENetApp VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(Common)
add_subdirectory(ClientApp)
add_subdirectory(ServerApp)
