add_library(controller "")

target_sources(controller
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/controller.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/controller.hpp
  )

target_include_directories(controller PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(controller
  PRIVATE
    pdcursesadapter
  )