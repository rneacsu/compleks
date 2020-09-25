configure_file(main.rc.in main.rc)

add_executable(main
    main.cpp bootstrap.cxx worlds/demo_world.cxx
    ${CMAKE_CURRENT_BINARY_DIR}/main.rc)

if(CMAKE_BUILD_TYPE MATCHES Release)
    set_property(TARGET main PROPERTY WIN32_EXECUTABLE True)
endif()

target_link_libraries(main compleks)
