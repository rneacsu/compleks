cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0091 NEW)

set(VCPKG_TARGET_TRIPLET x64-windows-static CACHE STRING "Vcpkg target triplet")

project(compleks VERSION 1.0.0)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS stb.h REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS 
    "/EHsc /W4 /WX \
    /experimental:external /external:anglebrackets /external:W0 \
    /std:c++17"
)

set(CMAKE_MSVC_RUNTIME_LIBRARY 
    "MultiThreaded$<$<CONFIG:Debug>:Debug>")
if (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_WIN32_EXECUTABLE True)
endif()

add_subdirectory(src)

install(TARGETS main RUNTIME COMPONENT app)
install(DIRECTORY res DESTINATION . COMPONENT app)

set(CPACK_GENERATOR ZIP NSIS)
set(CPACK_PACKAGE_NAME "Compleks")
set(CPACK_PACKAGE_VENDOR "Razvan Neacsu")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Compleks")
set(CPACK_PACKAGE_EXECUTABLES "main;Compleks")

set(CPACK_COMPONENTS_ALL app)
set(CPACK_COMPONENT_APP_REQUIRED True)
set(CPACK_COMPONENT_APP_DISPLAY_NAME "Compleks")

set(CPACK_NSIS_MUI_ICON ${CMAKE_SOURCE_DIR}/res/app.ico)
set(CPACK_NSIS_MUI_FINISHPAGE_RUN main.exe)
set(CPACK_NSIS_INSTALLED_ICON_NAME bin/main.exe)
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL On)

include(CPack)
