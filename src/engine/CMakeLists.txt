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
    BulletCollision BulletDynamics
    nlohmann_json::nlohmann_json
)

if (CMAKE_BUILD_TYPE MATCHES Release)
    target_link_libraries(compleks
        ${BULLET_LIBRARY_DIRS}/LinearMath.lib
        ${BULLET_LIBRARY_DIRS}/Bullet3Common.lib
    )
else()
    target_link_libraries(compleks
        ${BULLET_LIBRARY_DIRS}/../debug/lib/LinearMath_Debug.lib
        ${BULLET_LIBRARY_DIRS}/../debug/lib/Bullet3Common_Debug.lib
    )
endif()
