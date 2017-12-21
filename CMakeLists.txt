cmake_minimum_required (VERSION 3.10)

project(GreenEngine)

set (GreenEngine_VERSION_MAJOR 1)
set (GreenEngine_VERSION_MINOR 0)

file (GLOB_RECURSE SOURCES "src/*.cpp")
file (GLOB_RECURSE HEADERS "src/*.h")

set (INCLUDE_DIRS "")
foreach (_headerFile ${HEADDERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

add_executable (GreenEngine src/main.cpp)

target_include_directories(GreenEngine PRIVATE ${INCLUDE_DIRS})