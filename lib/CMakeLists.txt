aux_source_directory(./src src)
aux_source_directory(./src/fabtex src_fabtex)
add_library(fabtex ${src} ${src_fabtex})
target_include_directories(fabtex PUBLIC include)
