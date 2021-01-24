configure_file(main.rc.in main.rc)

add_executable(main
    main.cpp
    bootstrap.cxx
    demo_world.cxx
    scenes/scene1.cxx
    scenes/scene2.cxx
    scenes/scene3.cxx
    scenes/scene4.cxx
    scenes/scene5.cxx
    scenes/scene6.cxx
    shapes/gate_shape.cxx
    ${CMAKE_CURRENT_BINARY_DIR}/main.rc)

target_link_libraries(main compleks)
