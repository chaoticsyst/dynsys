cmake_minimum_required(VERSION 3.15)

project(DynamicSystems)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Ofast -std=c++17 -pipe -Wall -Wextra")

include_directories("include/")

find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(OpenGL)

add_executable(DynamicSystems
    src/main.cpp
    src/Model.cpp
    src/Camera.cpp
    src/PointsViewQGLWidget.cpp
    src/Window.cpp
    src/Locus.cpp
    include/Camera.h
    include/Model.h
    include/Window.h
    include/PointsViewQGLWidget.h
    include/Locus.h
    src/form.ui
)

qt5_use_modules(DynamicSystems Widgets OpenGL)

target_link_libraries(DynamicSystems ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
