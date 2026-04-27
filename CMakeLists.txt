cmake_minimum_required(VERSION 3.16)
project(MazeGenerator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(HEADERS
        ${INCLUDE_DIR}/difficultysettings.h
        ${INCLUDE_DIR}/mainwindow.h
        ${INCLUDE_DIR}/mazegenerator.h
        ${INCLUDE_DIR}/mazewidget.h
        ${INCLUDE_DIR}/pathfinder.h
)

set(SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/mainwindow.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${SRC_DIR}/mazewidget.cpp
        ${SRC_DIR}/pathfinder.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

add_executable(MazeGenerator WIN32 ${SOURCES} ${HEADERS})

target_include_directories(MazeGenerator PRIVATE ${INCLUDE_DIR})

target_link_libraries(MazeGenerator PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
)

if(WIN32)
    set_target_properties(MazeGenerator PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()