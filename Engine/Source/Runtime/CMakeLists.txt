file(GLOB_RECURSE RUNTIME_SOURCE "${PROJECT_SOURCE_DIR}/Engine/Source/Runtime/*.cpp")
file(GLOB_RECURSE RUNTIME_HEADER "${PROJECT_SOURCE_DIR}/Engine/Source/Runtime/*.h")

add_library(EngineRuntime STATIC ${RUNTIME_SOURCE} ${RUNTIME_HEADER})

target_link_libraries(EngineRuntime
        glad::glad
        glfw
        imgui::imgui
        spdlog::spdlog
        assimp::assimp
)

target_include_directories(EngineRuntime PUBLIC ${Stb_INCLUDE_DIR})
target_include_directories(EngineRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})