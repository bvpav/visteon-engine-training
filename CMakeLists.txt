cmake_minimum_required(VERSION 3.12)
project(visteontraining VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(engine
    src/main.cc
)

# GLFW
set(GLFW_BUILD_WAYLAND ON)
# set(GLFW_BUILD_X11 OFF)
add_subdirectory(thirdparty/glfw/)
target_link_libraries(engine PRIVATE glfw)

# GLEW
# Set the path to the GLEW source directory
set(GLEW_SOURCE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glew
)
get_property(GENERATOR GLOBAL PROPERTY GENERATOR)
# Add a custom target to build GLEW using CMake
# HACK: How can I instead get the "Unix Makefiles" parameter from the current CMake instance?
add_custom_target(build_glew
    COMMAND ${CMAKE_COMMAND} -E chdir ${GLEW_SOURCE_DIR}/build ${CMAKE_COMMAND} -G "Unix Makefiles" cmake/
    COMMAND ${CMAKE_COMMAND} --build ${GLEW_SOURCE_DIR}/build
    WORKING_DIRECTORY ${GLEW_SOURCE_DIR}
    COMMENT "Building GLEW"
)

# Add a dependency to make sure GLEW is built before your target
add_dependencies(engine build_glew)

target_include_directories(engine PRIVATE
    ${GLEW_SOURCE_DIR}/include
)
target_link_libraries(engine PRIVATE
    ${GLEW_SOURCE_DIR}/lib/libGLEW.a
)

if (WIN32)
    target_link_libraries(engine PRIVATE opengl32)
elseif (UNIX)
    target_link_libraries(engine PRIVATE GL)
endif()