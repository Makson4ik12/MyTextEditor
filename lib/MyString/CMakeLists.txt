add_library(mystring SHARED mystring.cpp mystring.h)

target_include_directories(mystring PUBLIC ${CMAKE_CURRENT_LIST_DIR})