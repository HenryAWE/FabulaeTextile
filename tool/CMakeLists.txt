find_package(SDL2 REQUIRED)
add_subdirectory(imgui)

aux_source_directory(. visualizer_src)

add_executable(visualizer WIN32 ${visualizer_src})

target_link_libraries(visualizer PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(visualizer PRIVATE imgui)
target_link_libraries(visualizer PRIVATE fabtex)
