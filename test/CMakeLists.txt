cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Ofast -pipe -Wall -Wextra")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D__STDC_CONSTANT_MACROS")

project(TestDynSys)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(
    ../include/
    ${GTEST_INCLUDE_DIRS}        
)

find_package(Qt5Widgets REQUIRED)

add_executable(TestDynSys
    testAll.cpp
    testParser.cpp
    ../src/DynamicSystemParser/DynamicSystemParser.cpp
    ../src/Parser/Parser.cpp
    ../src/Parser/Lexer.cpp
    testSystems.cpp
)

qt5_use_modules(TestDynSys Widgets OpenGL)

target_link_libraries(TestDynSys
    ${QT_LIBRARIES}
    ${GTEST_LIBRARIES}
    Threads::Threads
)

enable_testing()
add_Test(TestDynSys "./TestDynSys")
