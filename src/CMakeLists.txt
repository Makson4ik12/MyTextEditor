add_executable(mytexteditor main.cpp )
  
add_subdirectory(controller)
add_subdirectory(view)
add_subdirectory(model)
add_subdirectory(log)

target_link_libraries(mytexteditor
  PRIVATE
    mystring
    pdcurses
    controller
    model
    view
    pdcursesadapter
    log
  )