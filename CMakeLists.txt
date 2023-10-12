cmake_minimum_required(VERSION 3.1)
project(dennm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Torch REQUIRED)

add_executable(main src/test_script.cpp)

target_link_libraries(main ${TORCH_LIBRARIES})

target_include_directories(main PRIVATE ${TORCH_INCLUDE_DIRS})