cmake_minimum_required(VERSION 3.1)
set(PROJECT_NAME OpenGLViewer)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project (${PROJECT_NAME})

# adding sources to include in project
file(GLOB SOURCES "src/*.cpp" "src/*.h")

# adding required include directories
include_directories(GLEW/include/GL)
include_directories(GLFW/include/GLFW)
include_directories(GLM/glm)

# setting non required flags to OFF
set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

# adding dependencies to build
add_subdirectory(GLM)
add_subdirectory(GLFW)
add_subdirectory(GLEW/build/cmake)

# organizing in solution folder
set_target_properties(glew PROPERTIES FOLDER GLEW)
set_target_properties(glew_s PROPERTIES FOLDER GLEW)
set_target_properties(uninstall PROPERTIES FOLDER GLEW)
set_target_properties(glm_shared PROPERTIES FOLDER GLM)
set_target_properties(glm_static PROPERTIES FOLDER GLM)

# adding library to link
set(LIBRARIES_TO_LINK OpenGL32.lib)

add_executable(${PROJECT_NAME} ${SOURCES})

# adding reference to dependent projects
target_link_libraries(${PROJECT_NAME} ${LIBRARIES_TO_LINK} glfw)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES_TO_LINK} glew_s)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES_TO_LINK} glm_static)