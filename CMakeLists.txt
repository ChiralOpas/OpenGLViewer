cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME OpenGLViewer)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project (${PROJECT_NAME})

# adding sources to include in project
file(GLOB SOURCES "src/*.cpp" "src/*.h")

# adding required include directories
include_directories(GLEW/include/GL)
include_directories(GLFW/include/GLFW)

# setting non required flags to OFF
option (BUILD_UTILS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)


# adding dependencies to build
add_subdirectory(GLFW)
add_subdirectory(GLEW/build/cmake)

# organizing in solution folder
set_target_properties(glew PROPERTIES FOLDER GLEW)
set_target_properties(glew_s PROPERTIES FOLDER GLEW)
set_target_properties(uninstall PROPERTIES FOLDER GLEW)

# adding library to link
set(LIBRARIES_TO_LINK OpenGL32.lib)

add_executable(${PROJECT_NAME} ${SOURCES})

# adding reference to dependent projects
target_link_libraries(${PROJECT_NAME} ${LIBRARIES_TO_LINK} glfw)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES_TO_LINK} glew_s)