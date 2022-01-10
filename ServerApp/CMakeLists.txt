cmake_minimum_required(VERSION 3.22)

Project(CEENetAppServer VERSION 1.0.0 LANGUAGES CXX)

add_executable(Server.x86_64 Main.cpp)

target_link_libraries(Server.x86_64 PRIVATE Common)
