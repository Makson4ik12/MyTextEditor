add_library(pdcurses "")

file(GLOB PDCURSES_OBJECTS
    ${CMAKE_CURRENT_LIST_DIR}/*.o
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

target_sources(pdcurses
  PUBLIC
    ${PDCURSES_OBJECTS}
  )
target_include_directories(pdcurses PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set_target_properties(pdcurses PROPERTIES LINKER_LANGUAGE CXX)