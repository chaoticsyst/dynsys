cmake_minimum_required(VERSION 3.15)

project(DynamicSystems)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Ofast -pipe -Wall -Wextra")

include_directories(include/ include/Parser/)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)


set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVSWS_INCLUDE_DIR libswscale/swscale.h)
find_library(AVSWS_LIBRARY swscale)

add_executable(DynamicSystems
    src/main.cpp
    src/WindowPreferences.cpp
    src/Camera.cpp
    src/PointsViewQGLWidget.cpp
    src/Window.cpp
    src/Locus.cpp
    src/Preferences.cpp
    src/VideoEncoder.cpp
    src/ShaderController.cpp
    src/DynamicSystemParser/DynamicSystemParser.cpp
    src/Parser/Parser.cpp
    src/Parser/Lexer.cpp
    include/Preferences.hpp
    include/Camera.hpp
    include/WindowPreferences.hpp
    include/Model/Model.hpp
    include/Model/Impl/ModelImpl.hpp
    include/DynamicSystems/DynamicSystem.hpp
    include/DynamicSystems/Impl/SystemsBase.hpp
    include/DynamicSystems/Impl/SystemsBaseGetImpl.hpp
    include/DynamicSystems/Impl/DynamicSystemImpl.hpp
    include/DynamicSystems/Impl/DynamicSystemInternal.hpp
    include/DynamicSystemParser/DynamicSystemParser.hpp
    include/DynamicSystemParser/Impl/DynamicSystemParserImpl.hpp
    include/Window.hpp
    include/DynamicSystemWrapper.hpp
    include/PointsViewQGLWidget.hpp
    include/StoppableTask.hpp
    include/Locus.hpp
    include/VideoEncoder.hpp
    include/ShaderController.hpp
    include/Parser/Parser.hpp
    include/Parser/ParserException.hpp
    include/Parser/ParserNodes.hpp
    include/Parser/Lexer.hpp
    src/form.ui
    src/formPreferences.ui
    materials/Resources.qrc
)

qt5_use_modules(DynamicSystems Widgets OpenGL)

add_subdirectory(test/)

target_link_libraries(DynamicSystems
        ${QT_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${AVCODEC_LIBRARY}
        ${AVFORMAT_LIBRARY}
        ${AVUTIL_LIBRARY}
        ${AVSWS_LIBRARY}
        Threads::Threads
)
