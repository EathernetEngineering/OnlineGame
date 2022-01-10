cmake_minimum_required(VERSION 3.22)

Project(CEENetAppClient VERSION 1.0.0 LANGUAGES CXX)

add_executable(Client.x86_64 Main.cpp)

target_link_libraries(Client.x86_64 PRIVATE Common)
