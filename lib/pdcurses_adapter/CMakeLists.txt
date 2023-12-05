add_library(pdcursesadapter "")

target_sources(pdcursesadapter
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/pdcursesadapter.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/pdcursesadapter.hpp
  )

target_include_directories(pdcursesadapter PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(view
  PRIVATE
    mystring
  )