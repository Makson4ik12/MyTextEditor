add_library(view "")

target_sources(view
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/view.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/view.hpp
  )
target_include_directories(view PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(view
  PRIVATE
    pdcursesadapter
  )