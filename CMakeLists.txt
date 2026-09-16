cmake_minimum_required(VERSION 4.3)
project(GTK4_Notepad)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

add_executable(GtkSample main.cpp)

add_executable(GTK4_Notepad main.cpp)

target_link_libraries(GTK4_Notepad PRIVATE PkgConfig::GTK4)
