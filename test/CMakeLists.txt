find_package(GTest CONFIG REQUIRED)

add_subdirectory(bitmap)
add_subdirectory(core)
add_subdirectory(path)
add_subdirectory(vector)

add_custom_target(all_test)
add_dependencies(
    all_test
    bitmap_test
    core_test
    path_test
    vector_test
)
