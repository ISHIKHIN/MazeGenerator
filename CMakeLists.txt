cmake_minimum_required(VERSION 3.19)
project(MazeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Попробуем найти Qt6, если не получится - используем Qt5
find_package(Qt6 QUIET COMPONENTS Widgets)
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_VERSION_MAJOR 5)
    message(STATUS "Using Qt5")
else()
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Using Qt6")
endif()

set(SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mazewidget.cpp
        src/mazegenerator.cpp
        src/pathfinder.cpp
)

set(HEADERS
        include/maze/mainwindow.h
        include/maze/mazewidget.h
        include/maze/mazegenerator.h
        include/maze/pathfinder.h
        include/maze/difficultysettings.h
)

add_executable(MazeGenerator WIN32 ${SOURCES} ${HEADERS})

target_include_directories(MazeGenerator PRIVATE include)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(MazeGenerator PRIVATE Qt6::Widgets)
else()
    target_link_libraries(MazeGenerator PRIVATE Qt5::Widgets)
endif()