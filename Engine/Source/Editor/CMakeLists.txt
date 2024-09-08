file(GLOB_RECURSE EDITOR_SOURCE "*.cpp" "*.cu")
file(GLOB_RECURSE EDITOR_HEADER "*.h" "*.hpp" "*.cuh")

add_executable(Editor ${EDITOR_SOURCE} ${EDITOR_HEADER})
target_link_libraries(Editor EngineRuntime)
target_include_directories(Editor PRIVATE
        ${PROJECT_SOURCE_DIR}/Engine/Source/Runtime
)