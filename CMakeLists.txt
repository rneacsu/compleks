cmake_minimum_required(VERSION 3.10)
project(compleks VERSION 1.0.0)

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

install(TARGETS main compleks RUNTIME COMPONENT app)
install(DIRECTORY res DESTINATION . COMPONENT app)
install(CODE "
    file(GLOB APP_DLL \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/*.dll\")
    file(INSTALL \${APP_DLL} DESTINATION \${CMAKE_INSTALL_PREFIX}/bin)
" COMPONENT app)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT app)
include(InstallRequiredSystemLibraries)

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
