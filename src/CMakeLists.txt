add_executable(mytexteditor main.cpp )
  
add_subdirectory(controller)
add_subdirectory(view)
add_subdirectory(model)

target_link_libraries(mytexteditor
  PRIVATE
    mystring
    controller
    model
    view
    pdcursesadapter
    pdcurses
  )