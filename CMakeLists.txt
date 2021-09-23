cmake_minimum_required(VERSION 3.20)
project(botw_save_editor)

set(CMAKE_CXX_STANDARD 17)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fpermissive)
    SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Compiler options for Visual Studio
endif()

add_executable(botw_save_editor main.cpp json.h Parser/StringTableParser.cpp Parser/StringTableParser.cpp Parser/StringTableParser.h)
