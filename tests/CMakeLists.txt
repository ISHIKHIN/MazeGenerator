# Отключаем предупреждение о политике CMP0135
cmake_policy(SET CMP0135 NEW)

# Автоматическая загрузка GoogleTest
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Пути к исходникам и заголовкам
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Тестируемые исходники
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

# Заголовки
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

# Находим Qt6 для тестов
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

#-----------------------------------------------------------------------------
# Unit-тесты
#-----------------------------------------------------------------------------

# 1. Тесты для GeneratorSettings
add_executable(test_difficultysettings
        test_main.cpp
        test_difficultysettings.cpp
)
target_include_directories(test_difficultysettings PRIVATE ${INCLUDE_DIR})
target_link_libraries(test_difficultysettings PRIVATE gtest_main)
add_test(NAME DifficultySettingsTest COMMAND test_difficultysettings)
set_tests_properties(DifficultySettingsTest PROPERTIES LABELS "unit")

# 2. Тесты для MazeGenerator
add_executable(test_mazegenerator
        test_main.cpp
        test_mazegenerator.cpp
        ${TESTED_SOURCES}
        ${TEST_HEADERS}
)
target_include_directories(test_mazegenerator PRIVATE ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(test_mazegenerator PRIVATE gtest_main Qt6::Core)
add_test(NAME MazeGeneratorTest COMMAND test_mazegenerator)
set_tests_properties(MazeGeneratorTest PROPERTIES LABELS "unit")

# 3. Тесты для PathFinder
add_executable(test_pathfinder
        test_main.cpp
        test_pathfinder.cpp
        ${TESTED_SOURCES}
        ${TEST_HEADERS}
)
target_include_directories(test_pathfinder PRIVATE ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(test_pathfinder PRIVATE gtest_main Qt6::Core)
add_test(NAME PathFinderTest COMMAND test_pathfinder)
set_tests_properties(PathFinderTest PROPERTIES LABELS "unit")

# 4. Тесты для MainWindow
add_executable(test_mainwindow
        test_main.cpp
        test_mainwindow.cpp
        ${MAINWINDOW_SOURCES}
        ${MAINWINDOW_HEADERS}
)
target_include_directories(test_mainwindow PRIVATE ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(test_mainwindow PRIVATE gtest_main Qt6::Core Qt6::Widgets Qt6::Gui)
add_test(NAME MainWindowTest COMMAND test_mainwindow)
set_tests_properties(MainWindowTest PROPERTIES LABELS "unit")

# 5. Тесты для MazeWidget
add_executable(test_mazewidget
        test_main.cpp
        test_mazewidget.cpp
        ${MAZEWIDGET_SOURCES}
        ${MAZEWIDGET_HEADERS}
)
target_include_directories(test_mazewidget PRIVATE ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(test_mazewidget PRIVATE gtest_main Qt6::Core Qt6::Widgets Qt6::Gui)
add_test(NAME MazeWidgetTest COMMAND test_mazewidget)
set_tests_properties(MazeWidgetTest PROPERTIES LABELS "unit")

#-----------------------------------------------------------------------------
# Тестовые сценарии
#-----------------------------------------------------------------------------

# Сценарий 1: Полная генерация и поиск пути
add_executable(scenario_full_generation
        scenarios/scenario_full_generation.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${SRC_DIR}/pathfinder.cpp
        ${TEST_HEADERS}
)
target_include_directories(scenario_full_generation PRIVATE ${INCLUDE_DIR})
target_link_libraries(scenario_full_generation PRIVATE Qt6::Core)
add_test(NAME Scenario_FullGeneration COMMAND scenario_full_generation)

# Сценарий 2: Поиск пути в различных условиях
add_executable(scenario_path_search
        scenarios/scenario_path_search.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${SRC_DIR}/pathfinder.cpp
        ${TEST_HEADERS}
)
target_include_directories(scenario_path_search PRIVATE ${INCLUDE_DIR})
target_link_libraries(scenario_path_search PRIVATE Qt6::Core)
add_test(NAME Scenario_PathSearch COMMAND scenario_path_search)

# Сценарий 3: Смена сложности
add_executable(scenario_difficulty_switch
        scenarios/scenario_difficulty_switch.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${TEST_HEADERS}
)
target_include_directories(scenario_difficulty_switch PRIVATE ${INCLUDE_DIR})
target_link_libraries(scenario_difficulty_switch PRIVATE Qt6::Core)
add_test(NAME Scenario_DifficultySwitch COMMAND scenario_difficulty_switch)

# Сценарий 4: Генерация больших лабиринтов
add_executable(scenario_large_maze
        scenarios/scenario_large_maze.cpp
        ${SRC_DIR}/mazegenerator.cpp
        ${TEST_HEADERS}
)
target_include_directories(scenario_large_maze PRIVATE ${INCLUDE_DIR})
target_link_libraries(scenario_large_maze PRIVATE Qt6::Core)
add_test(NAME Scenario_LargeMaze COMMAND scenario_large_maze)