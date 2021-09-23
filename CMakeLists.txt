cmake_minimum_required(VERSION 3.20)
project(botw_save_editor)

set(CMAKE_CXX_STANDARD 17)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fpermissive)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Compiler options for Visual Studio
endif()

add_executable(botw_save_editor main.cpp json.h)
