cmake_minimum_required(VERSION 3.16)
project(MazeGenerator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ---------- Пути ----------
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)

# ---------- Qt6 ----------
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# ---------- GoogleTest ----------
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

# ---------- Основное приложение ----------
set(APP_SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/mainwindow.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${SRC_DIR}/mazewidget.cpp
        ${SRC_DIR}/pathfinder.cpp
)

set(APP_HEADERS
        ${INCLUDE_DIR}/difficultysettings.h
        ${INCLUDE_DIR}/mainwindow.h
        ${INCLUDE_DIR}/mazegenerator.h
        ${INCLUDE_DIR}/mazewidget.h
        ${INCLUDE_DIR}/pathfinder.h
)

add_executable(MazeGenerator WIN32 ${APP_SOURCES} ${APP_HEADERS})
target_include_directories(MazeGenerator PRIVATE ${INCLUDE_DIR})
target_link_libraries(MazeGenerator PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)

# ---------- Тестируемые исходники ----------
set(TESTED_SOURCES
        ${SRC_DIR}/mazegenerator.cpp
        ${SRC_DIR}/pathfinder.cpp
)

set(MAINWINDOW_SOURCES
        ${SRC_DIR}/mainwindow.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${SRC_DIR}/mazewidget.cpp
        ${SRC_DIR}/pathfinder.cpp
)

set(MAZEWIDGET_SOURCES
        ${SRC_DIR}/mazewidget.cpp
)

set(TEST_HEADERS
        ${INCLUDE_DIR}/difficultysettings.h
        ${INCLUDE_DIR}/mazegenerator.h
        ${INCLUDE_DIR}/pathfinder.h
)

set(MAINWINDOW_HEADERS
        ${INCLUDE_DIR}/mainwindow.h
        ${INCLUDE_DIR}/mazegenerator.h
        ${INCLUDE_DIR}/mazewidget.h
        ${INCLUDE_DIR}/pathfinder.h
        ${INCLUDE_DIR}/difficultysettings.h
)

set(MAZEWIDGET_HEADERS
        ${INCLUDE_DIR}/mazewidget.h
)

# ---------- Unit-тесты (из папки tests) ----------
add_executable(test_difficultysettings
        ${TESTS_DIR}/test_main.cpp
        ${TESTS_DIR}/test_difficultysettings.cpp
)
target_include_directories(test_difficultysettings PRIVATE ${INCLUDE_DIR})
target_link_libraries(test_difficultysettings PRIVATE gtest_main)

add_executable(test_mazegenerator
        ${TESTS_DIR}/test_main.cpp
        ${TESTS_DIR}/test_mazegenerator.cpp
        ${TESTED_SOURCES}
        ${TEST_HEADERS}
)
target_include_directories(test_mazegenerator PRIVATE ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(test_mazegenerator PRIVATE gtest_main Qt6::Core)

add_executable(test_pathfinder
        ${TESTS_DIR}/test_main.cpp
        ${TESTS_DIR}/test_pathfinder.cpp
        ${TESTED_SOURCES}
        ${TEST_HEADERS}
)
target_include_directories(test_pathfinder PRIVATE ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(test_pathfinder PRIVATE gtest_main Qt6::Core)

add_executable(test_mainwindow
        ${TESTS_DIR}/test_main.cpp
        ${TESTS_DIR}/test_mainwindow.cpp
        ${MAINWINDOW_SOURCES}
        ${MAINWINDOW_HEADERS}
)
target_include_directories(test_mainwindow PRIVATE ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(test_mainwindow PRIVATE gtest_main Qt6::Core Qt6::Widgets Qt6::Gui)

add_executable(test_mazewidget
        ${TESTS_DIR}/test_main.cpp
        ${TESTS_DIR}/test_mazewidget.cpp
        ${MAZEWIDGET_SOURCES}
        ${MAZEWIDGET_HEADERS}
)
target_include_directories(test_mazewidget PRIVATE ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(test_mazewidget PRIVATE gtest_main Qt6::Core Qt6::Widgets Qt6::Gui)

# ---------- Сценарии (из папки tests/scenarios) ----------
add_executable(scenario_full_generation
        ${TESTS_DIR}/scenarios/scenario_full_generation.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${SRC_DIR}/pathfinder.cpp
        ${TEST_HEADERS}
)
target_include_directories(scenario_full_generation PRIVATE ${INCLUDE_DIR})
target_link_libraries(scenario_full_generation PRIVATE Qt6::Core)

add_executable(scenario_path_search
        ${TESTS_DIR}/scenarios/scenario_path_search.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${SRC_DIR}/pathfinder.cpp
        ${TEST_HEADERS}
)
target_include_directories(scenario_path_search PRIVATE ${INCLUDE_DIR})
target_link_libraries(scenario_path_search PRIVATE Qt6::Core)

add_executable(scenario_difficulty_switch
        ${TESTS_DIR}/scenarios/scenario_difficulty_switch.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${TEST_HEADERS}
)
target_include_directories(scenario_difficulty_switch PRIVATE ${INCLUDE_DIR})
target_link_libraries(scenario_difficulty_switch PRIVATE Qt6::Core)

add_executable(scenario_large_maze
        ${TESTS_DIR}/scenarios/scenario_large_maze.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${TEST_HEADERS}
)
target_include_directories(scenario_large_maze PRIVATE ${INCLUDE_DIR})
target_link_libraries(scenario_large_maze PRIVATE Qt6::Core)

# Включить тесты в CTest
enable_testing()
add_test(NAME DifficultySettingsTest COMMAND test_difficultysettings)
add_test(NAME MazeGeneratorTest COMMAND test_mazegenerator)
add_test(NAME PathFinderTest COMMAND test_pathfinder)
add_test(NAME MainWindowTest COMMAND test_mainwindow)
add_test(NAME MazeWidgetTest COMMAND test_mazewidget)
add_test(NAME Scenario_FullGeneration COMMAND scenario_full_generation)
add_test(NAME Scenario_PathSearch COMMAND scenario_path_search)
add_test(NAME Scenario_DifficultySwitch COMMAND scenario_difficulty_switch)
add_test(NAME Scenario_LargeMaze COMMAND scenario_large_maze)