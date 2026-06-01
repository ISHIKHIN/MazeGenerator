# ========== СТАДИЯ 1: СБОРКА ==========
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Установка зависимостей для сборки
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-tools-dev \
    libgl1-mesa-dev \
    libxcb-*-0 \
    libx11-xcb1 \
    xvfb \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY . .

# Сборка проекта и тестов
RUN mkdir -p build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Запуск тестов (каждый тест выполняется отдельно, сборка не прерывается)
RUN cd build && \
    echo "==========================================" && \
    echo "ЗАПУСК ТЕСТОВ" && \
    echo "==========================================" && \
    (xvfb-run -a ./test_difficultysettings && echo "✓ test_difficultysettings PASSED" || echo "✗ test_difficultysettings FAILED") && \
    (xvfb-run -a ./test_mazegenerator && echo "✓ test_mazegenerator PASSED" || echo "✗ test_mazegenerator FAILED") && \
    (xvfb-run -a ./test_pathfinder && echo "✓ test_pathfinder PASSED" || echo "✗ test_pathfinder FAILED") && \
    (xvfb-run -a ./test_mainwindow && echo "✓ test_mainwindow PASSED" || echo "✗ test_mainwindow FAILED") && \
    (xvfb-run -a ./test_mazewidget && echo "✓ test_mazewidget PASSED" || echo "✗ test_mazewidget FAILED") && \
    echo "==========================================" && \
    echo "ЗАПУСК СЦЕНАРИЕВ" && \
    echo "==========================================" && \
    (./scenario_full_generation && echo "✓ scenario_full_generation PASSED" || echo "✗ scenario_full_generation FAILED") && \
    (./scenario_path_search && echo "✓ scenario_path_search PASSED" || echo "✗ scenario_path_search FAILED") && \
    (./scenario_difficulty_switch && echo "✓ scenario_difficulty_switch PASSED" || echo "✗ scenario_difficulty_switch FAILED") && \
    (./scenario_large_maze && echo "✓ scenario_large_maze PASSED" || echo "✗ scenario_large_maze FAILED") && \
    echo "==========================================" && \
    echo "ТЕСТИРОВАНИЕ ЗАВЕРШЕНО" && \
    echo "=========================================="

# Опционально: можно проверить что все тесты прошли и выйти с ошибкой если нет
# RUN cd build && \
#     (xvfb-run -a ./test_mainwindow 2>&1 | grep -q "PASSED" && exit 0 || exit 1)

# ========== СТАДИЯ 2: ФИНАЛЬНЫЙ ОБРАЗ ==========
FROM ubuntu:22.04

# Только минимальные runtime зависимости
RUN apt-get update && apt-get install -y \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    libgl1-mesa-glx \
    libxcb-*-0 \
    libx11-xcb1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /build/build/MazeGenerator /usr/local/bin/
RUN chmod +x /usr/local/bin/MazeGenerator

CMD ["/usr/local/bin/MazeGenerator"]