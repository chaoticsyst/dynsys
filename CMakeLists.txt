cmake_minimum_required(VERSION 3.5)

project(dynsys LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")

include_directories("include/")

find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(OpenGL)

add_executable(dynsys
    src/main.cpp
    src/Model.cpp
    src/mainwindow.cpp
    include/Model.h
    include/mainwindow.h
    mainwindow.ui
)

qt5_use_modules(dynsys Widgets OpenGL)

target_link_libraries(dynsys ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
