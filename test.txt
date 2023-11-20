cmake_minimum_required(VERSION 3.28)
project(MyTextEditor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(LIB_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib) # Set the path to the DLL output directory
set(MYSTRING_LIB mystring)
set(PDCURSES_LIB pdcurses)

add_library(${MYSTRING_LIB} SHARED lib/mystring.cpp lib/mystring.h) # Add the library target
add_library(${PDCURSES_LIB} STATIC lib/PDCursesAdapter.cpp lib/PDCursesAdapter.hpp) # Add the library target

set_target_properties(${MYSTRING_LIB} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR}
)

target_include_directories(${MYSTRING_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib) # Add the library directory to include search paths
target_include_directories(${PDCURSES_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Main executable
set(SOURCE_FILES main.cpp)
add_executable(vim ${SOURCE_FILES})

target_link_libraries(vim PRIVATE ${MYSTRING_LIB}) # Link the executable with the library