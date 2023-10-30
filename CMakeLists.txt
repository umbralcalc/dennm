cmake_minimum_required(VERSION 3.1)
project(dennm-torch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(CTest)
# find_package(GTest REQUIRED)

set(SRC_FILES
    src/main.cpp
    src/SecondOrderProbsState.cpp
    src/SecondOrderProbsState.h
    src/ThirdOrderProbsState.cpp
    src/ThirdOrderProbsState.h
)

add_executable(dennm-torch ${SRC_FILES})

find_package(Torch REQUIRED)
target_link_libraries(dennm-torch ${TORCH_LIBRARIES})
target_include_directories(dennm-torch PRIVATE ${TORCH_INCLUDE_DIRS})

# add_executable(test-dennm-torch test/tests.cpp)

# target_link_libraries(test-dennm-torch GTest::GTest GTest::Main)
# target_link_libraries(test-dennm-torch ${TORCH_LIBRARIES})
# target_include_directories(test-dennm-torch PRIVATE ${GTEST_INCLUDE_DIRS})
# target_include_directories(test-dennm-torch PRIVATE ${TORCH_INCLUDE_DIRS})

# add_test(NAME test-dennm-torch COMMAND test-dennm-torch)