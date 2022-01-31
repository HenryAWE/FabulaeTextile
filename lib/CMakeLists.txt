aux_source_directory(src fabtex_src)
add_library(fabtex ${fabtex_src})
target_include_directories(fabtex PUBLIC include)
