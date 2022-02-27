cmake_minimum_required(VERSION 3.20)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    message(STATUS "Use vcpkg toolchain from $ENV{VCPKG_ROOT}")
endif()

# Compiler options
# Use C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Force UTF-8 on MSVC
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# Make sure macro "__cplusplus" is correctly defined on MSVC
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")

project(
    "Fabulae Textile"
    VERSION 0.1
    LANGUAGES C CXX
)

add_subdirectory(lib)
option(FABTEX_BUILD_TOOL "Build tool" ON)
if(${FABTEX_BUILD_TOOL})
    add_subdirectory(tool)
endif()
option(FABTEX_BUILD_TEST "Build the test suites" ON)
if(${FABTEX_BUILD_TEST})
    enable_testing()
    add_subdirectory(test)
endif()
