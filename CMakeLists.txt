cmake_minimum_required(VERSION 3.26)

project(hello-start)

set(CMAKE_CXX_STANDARD 11) # 指定使用C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 确保C++标准必须被支持
set(CMAKE_CXX_EXTENSIONS OFF) # 关闭编译器特定的扩展，使用标准的C++

set(Boost_INCLUDE_DIR "E:/dev/boost/boost_1_84_0")
set(Boost_LIBRARY_DIR "E:/dev/boost/boost_1_84_0/libs")

add_subdirectory(start)
add_subdirectory(asio)
add_subdirectory(channel)
