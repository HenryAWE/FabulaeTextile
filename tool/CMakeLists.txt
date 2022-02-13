find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)
add_subdirectory(imgui)
add_subdirectory(stb)

aux_source_directory(. visualizer_src)

add_executable(visualizer WIN32 ${visualizer_src})

target_link_libraries(visualizer PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(visualizer PRIVATE Threads::Threads)
target_link_libraries(visualizer PRIVATE imgui)
target_link_libraries(visualizer PRIVATE stb)
target_link_libraries(visualizer PRIVATE fabtex)
