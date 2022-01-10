cmake_minimum_required(VERSION 3.22)

Project(CEENetAppCommon VERSION 1.0.0 LANGUAGES CXX)

add_library(Common INTERFACE)

target_include_directories(Common INTERFACE include/)
target_link_libraries(Common INTERFACE pthread)
