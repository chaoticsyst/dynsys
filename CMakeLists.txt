cmake_minimum_required(VERSION 3.15)

project(DynamicSystems)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -Wall -Wextra")

include_directories("include/")

find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(OpenGL)

add_executable(DynamicSystems
    src/main.cpp
    src/Model.cpp
    src/visual_opengl_widget.cpp
    src/window.cpp
    include/Model.h
    include/window.h
    include/visual_opengl_widget.h
    src/form.ui
)

qt5_use_modules(DynamicSystems Widgets OpenGL)

target_link_libraries(DynamicSystems ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
