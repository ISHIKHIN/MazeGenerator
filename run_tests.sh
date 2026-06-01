#!/bin/bash
set -e

echo "=========================================="
echo "ЗАПУСК UNIT-ТЕСТОВ"
echo "=========================================="

xvfb-run -a /usr/local/bin/test_difficultysettings
xvfb-run -a /usr/local/bin/test_mazegenerator
xvfb-run -a /usr/local/bin/test_pathfinder
xvfb-run -a /usr/local/bin/test_mainwindow
xvfb-run -a /usr/local/bin/test_mazewidget

echo ""
echo "=========================================="
echo "ЗАПУСК СЦЕНАРИЕВ"
echo "=========================================="

echo "--- Scenario 1: Full Generation ---"
xvfb-run -a /usr/local/bin/scenario_full_generation

echo ""
echo "--- Scenario 2: Path Search ---"
xvfb-run -a /usr/local/bin/scenario_path_search

echo ""
echo "--- Scenario 3: Difficulty Switch ---"
xvfb-run -a /usr/local/bin/scenario_difficulty_switch

echo ""
echo "--- Scenario 4: Large Maze ---"
xvfb-run -a /usr/local/bin/scenario_large_maze

echo ""
echo "=========================================="
echo "ВСЕ ТЕСТЫ И СЦЕНАРИИ УСПЕШНО ПРОЙДЕНЫ"
echo "=========================================="