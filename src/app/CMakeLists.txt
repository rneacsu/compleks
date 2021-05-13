configure_file(main.rc.in main.rc)

add_subdirectory(shapes)

add_executable(main
    main.cpp
    bootstrap.cxx
    demo.cxx
    overlay.cxx
    ${CMAKE_CURRENT_BINARY_DIR}/main.rc)

target_link_libraries(main app_shapes compleks)
