add_library(log "")

target_sources(log
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/log.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/log.hpp
  )

target_include_directories(log PUBLIC ${CMAKE_CURRENT_LIST_DIR})