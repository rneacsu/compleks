cmake_minimum_required(VERSION 3.10)
project(compleks)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_path(STB_INCLUDE_DIR NAMES stb_image.h REQUIRED)

set(CMAKE_CXX_FLAGS 
    "/EHsc /W4 /WX \
    /experimental:external /external:anglebrackets /external:W0 \
    /std:c++17"
)

add_subdirectory(src)
