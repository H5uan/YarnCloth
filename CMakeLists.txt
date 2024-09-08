cmake_minimum_required(VERSION 3.28)

project(XPBD-Cloth VERSION 0.1.0 LANGUAGES C CXX CUDA)
if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)

    add_definitions(-DUSE_CUDA)

    include_directories("${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")
endif()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,${PROJECT_NAME}/Debug,${PROJECT_NAME}/$<CONFIG>>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,${PROJECT_NAME}/Debug,${PROJECT_NAME}/$<CONFIG>>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<IF:$<CONFIG:Debug>,${PROJECT_NAME}/Debug,${PROJECT_NAME}f/$<CONFIG>>)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(Stb REQUIRED)
find_package(CUDAToolkit REQUIRED)



add_subdirectory(Engine/Source)