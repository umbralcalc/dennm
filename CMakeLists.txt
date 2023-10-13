cmake_minimum_required(VERSION 3.1)
project(dennm-torch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Torch REQUIRED)

add_executable(dennm-torch src/TestScript.cpp)

target_link_libraries(dennm-torch ${TORCH_LIBRARIES})

target_include_directories(dennm-torch PRIVATE ${TORCH_INCLUDE_DIRS})