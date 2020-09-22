configure_file(main.rc.in main.rc)

add_executable(main 
    main.cpp bootstrap.cxx worlds/demo_world.cxx
    ${CMAKE_CURRENT_BINARY_DIR}/main.rc)

target_link_libraries(main compleks)
