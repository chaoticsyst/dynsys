cmake_minimum_required(VERSION 3.15)

project(TestDynSys)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ../include
    ${GTEST_INCLUDE_DIRS}        
)

add_executable(TestDynSys test_all.cpp ../src)

target_link_libraries(TestDynSys ${GTEST_LIBRARIES} Threads::Threads)

enable_testing()
add_Test(TestDynSys "./TestDynSys")
