# 指定项目最小支持的cmake版本
cmake_minimum_required(VERSION 3.2)

# 项目名称
project(mystl)

# 增加-std=c++11
set(CMAKE_CXX_STANDARD 11)

# 头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/source)

# 搜索目录下的源文件，存储到变量中
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/test/vector_test.cpp)

# 生成一个可执行文件 app.exe 或 app.out
add_executable(main ${SRC})