cmake_minimum_required(VERSION 3.15)

project(DynamicSystems)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Ofast -pipe -Wall -Wextra")

include_directories("include/")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(DynamicSystems
    src/main.cpp
    src/WindowPreferences.cpp
    src/Camera.cpp
    src/PointsViewQGLWidget.cpp
    src/Window.cpp
    src/Locus.cpp
    src/Preferences.cpp
    src/Parser.cpp
    include/Parser.hpp
    include/ParserImpl.hpp
    include/AttractorsParams.h
    include/Preferences.h
    include/Camera.h
    include/WindowPreferences.h
    include/Model.hpp
    include/ModelImpl.hpp
    include/DynamicSystemsDefault.hpp
    include/Window.h
    include/PointsViewQGLWidget.h
    include/Locus.h
    src/form.ui
    src/formPreferences.ui
)

qt5_use_modules(DynamicSystems Widgets OpenGL)

add_subdirectory(test/)

target_link_libraries(DynamicSystems ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
