# Project Fabulae Textile / 锦织座计划
使用汉字部首和结构描述符生成汉字字体图像的拼字程序  
Simple program for generating images of Chinese characters by components and structural descriptors

# Library Design / 库设计
[`doc/design.md`](doc/design.md)

# External Dependencies
## Main Library
| Library Name | Version (At Least) | License |
|---|---|---|
| RapdidXml | 1.13 | Boost Software License |
## Visualizer
| Library Name | Version (At Least) | License | Note |
|---|---|---|---|
| SDL | 2.0.17+ | zlib |  |
| ImGui | 1.87 | MIT | included in source |
| stb | `af1a5bc` | Public Domain | included in source |
## Tests
| Library Name | Version (At Least) | License |
|---|---|---|
| googletest | 1.11.0 | BSD-3-Clause |

# 构建 / Build
## 安装外部依赖 / Install External Dependencies
- 根据需要安装上文列出的外部依赖项。推荐 Windows 平台上的用户使用 vcpkg 安装依赖项。`CMakeLists.txt` 会自动通过环境变量检测 vcpkg 并在检测到之后使用其工具链  
  Install dependencies listed above based on your needs. It's suggested that users on Windows platform using the *vcpkg*. `CMakeLists.txt` will automatically detects *vcpkg* by environment variables and use its toolchain after detecting it.
## 准备编译环境 / Setup Compilation Environment
- CMake 3.20+
- 任意支持 C++20 的编译器 / Any compiler that supports C++20  
  测试过的 C++ 编译器 / Tested C++ compilers
  1. MSVC 19.29 (Visual Studio 2019)
  2. GCC 11.1

## 构建 / Build
``` sh
mkdir build
cd build
cmake ..
cmake --build .
```
## 运行测试 / Run Tests
``` sh
ctest
```
