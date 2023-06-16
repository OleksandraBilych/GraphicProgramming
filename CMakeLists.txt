cmake_minimum_required(VERSION 3.5)
project(GraphicProgramming LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

###
### Initialization
###
# Project parameters
set(PROJ_SRC)

# Add all files/folders under src folder automatically to the project
file(GLOB_RECURSE PROJ_SRC src/*.h src/*.cpp)

add_subdirectory(externalLib/glad)

set(glm_DIR externalLib/GLM/cmake/glm)
find_package(glm REQUIRED)

###
### Generating the project files
###
add_executable(${PROJECT_NAME} ${PROJ_SRC} )

###
### Linking to the project
###
target_link_libraries(${PROJECT_NAME} glfw glad glm::glm)
