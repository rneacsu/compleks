include_directories(${GLEW_INCLUDE_DIRS} ${STB_INCLUDE_DIRS})

add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(input)
add_subdirectory(image)
add_subdirectory(render)
add_subdirectory(world)

add_library(compleks)

target_link_libraries(compleks
    compleks_core 
    compleks_utils
    compleks_input 
    compleks_image
    compleks_render
    compleks_world
)

target_link_libraries(compleks GLEW::GLEW glfw glm tinyobjloader::tinyobjloader)
