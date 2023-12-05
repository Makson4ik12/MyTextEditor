add_library(model "")

target_sources(model
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/model.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/model.hpp
  )
target_include_directories(model PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(model
  PRIVATE
    pdcursesadapter
    mystring
  )