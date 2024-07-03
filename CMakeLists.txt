cmake_minimum_required(VERSION 3.12)
project(visteontraining VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(engine
    src/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c
)

# GLFW
set(GLFW_BUILD_WAYLAND ON)
# set(GLFW_BUILD_X11 OFF)
add_subdirectory(thirdparty/glfw/)
target_link_libraries(engine PRIVATE glfw)

# GLAD
target_include_directories(engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
)
if (WIN32)
    target_link_libraries(engine PRIVATE opengl32)
elseif (UNIX)
    target_link_libraries(engine PRIVATE GL)
endif()