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

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
