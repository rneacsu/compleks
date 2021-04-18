add_subdirectory(window)
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(image)
add_subdirectory(render)
add_subdirectory(world)
add_subdirectory(physics)

add_library(compleks)

target_link_libraries(compleks
    compleks_window
    compleks_core
    compleks_utils
    compleks_image
    compleks_render
    compleks_world
    compleks_physics
)

target_link_libraries(compleks
    GLEW::GLEW
    glfw
    tinyobjloader::tinyobjloader
    LinearMath Bullet3Common BulletCollision BulletDynamics
    nlohmann_json::nlohmann_json
)
