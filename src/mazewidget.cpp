#include "maze/mazewidget.h"
#include <QPainter>
#include <QResizeEvent>
#include <QTimer>

MazeWidget::MazeWidget(QWidget *parent) : QWidget(parent) {
    mazeTimer = new QTimer(this);
    pathTimer = new QTimer(this);
    connect(mazeTimer, &QTimer::timeout, this, &MazeWidget::animateMazeStep);
    connect(pathTimer, &QTimer::timeout, this, &MazeWidget::animatePathStep);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MazeWidget::setMaze(const std::vector<std::vector<bool>>& maze, bool animate) {
    m_maze = maze;

    clearPath();

    if (animate && !m_maze.empty()) {
        startMazeAnimation();
    } else {
        if (mazeTimer->isActive()) mazeTimer->stop();
        animatingMaze = false;
        calculateOptimalCellSize();
        update();
    }
}

void MazeWidget::setPath(const std::vector<std::pair<int,int>>& path, bool animate) {
    m_path = path;
    if (animate && !m_path.empty()) {
        startPathAnimation();
    } else {
        if (pathTimer->isActive()) pathTimer->stop();
        animatingPath = false;
        update();
    }
}

void MazeWidget::clearPath() {
    if (pathTimer->isActive()) {
        pathTimer->stop();
    }

    animatingPath = false;
    m_path.clear();
    m_animatedPath.clear();
    animationStep = 0;
    update();
}

void MazeWidget::updateSize(const QSize& availableSize) {
    lastAvailableSize = availableSize;
    calculateOptimalCellSize();
}

void MazeWidget::resizeEvent(QResizeEvent *event) {
    lastAvailableSize = event->size();
    calculateOptimalCellSize();
    QWidget::resizeEvent(event);
}

void MazeWidget::calculateOptimalCellSize() {
    if (m_maze.empty()) return;

    int rows = m_maze.size();
    int cols = m_maze[0].size();

    if (rows == 0 || cols == 0) return;

    QSize available = lastAvailableSize.isValid() ? lastAvailableSize : size();
    if (available.width() <= 0 || available.height() <= 0) {
        available = QSize(800, 600);
    }

    int availableWidth = available.width() - 20;
    int availableHeight = available.height() - 20;

    int cellSizeByWidth = availableWidth / cols;
    int cellSizeByHeight = availableHeight / rows;

    int newCellSize = qMin(cellSizeByWidth, cellSizeByHeight);
    newCellSize = qMax(3, qMin(50, newCellSize));

    if (newCellSize != cellSize) {
        cellSize = newCellSize;
        setMinimumSize(cols * cellSize, rows * cellSize);
    }
}

void MazeWidget::startMazeAnimation() {
    animatingMaze = true;
    animationStep = 0;
    int rows = m_maze.size();
    int cols = m_maze[0].size();
    m_animatedMaze = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, true));
    mazeTimer->start(10);
}

void MazeWidget::startPathAnimation() {
    animatingPath = true;
    animationStep = 0;
    m_animatedPath.clear();
    pathTimer->start(50);
}

void MazeWidget::animateMazeStep() {
    if (!animatingMaze || animationStep >= (int)m_maze.size() * (int)m_maze[0].size()) {
        mazeTimer->stop();
        animatingMaze = false;
        m_animatedMaze = m_maze;
        calculateOptimalCellSize();
        update();
        return;
    }

    int rows = m_maze.size();
    int cols = m_maze[0].size();
    int y = animationStep / cols;
    int x = animationStep % cols;

    if (y < rows && x < cols) {
        m_animatedMaze[y][x] = m_maze[y][x];
    }

    animationStep++;
    update();
}

void MazeWidget::animatePathStep() {
    if (!animatingPath || animationStep >= (int)m_path.size()) {
        pathTimer->stop();
        animatingPath = false;
        m_animatedPath = m_path;
        update();
        return;
    }

    m_animatedPath.push_back(m_path[animationStep]);
    animationStep++;
    update();
}

void MazeWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const auto& mazeToDraw = animatingMaze ? m_animatedMaze : m_maze;
    if (mazeToDraw.empty()) return;

    int rows = mazeToDraw.size();
    int cols = mazeToDraw[0].size();

    painter.fillRect(rect(), QColor(240, 240, 240));

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            QRect cell(x * cellSize, y * cellSize, cellSize, cellSize);
            QColor color;
            if (mazeToDraw[y][x]) {
                color = QColor(50, 50, 50);
            } else if (y == 1 && x == 1) {
                color = QColor(76, 175, 80);
            } else if (y == rows - 2 && x == cols - 2) {
                color = QColor(244, 67, 54);
            } else {
                color = QColor(255, 248, 225);
            }
            painter.fillRect(cell, color);
        }
    }

    const auto& pathToDraw = animatingPath ? m_animatedPath : m_path;
    for (const auto& pos : pathToDraw) {
        int x = pos.first;
        int y = pos.second;
        if (!(y == 1 && x == 1) && !(y == rows - 2 && x == cols - 2)) {
            QRect cell(x * cellSize, y * cellSize, cellSize, cellSize);
            QColor pathColor(76, 175, 80, 180);
            painter.fillRect(cell, pathColor);

            painter.save();
            painter.setPen(QPen(QColor(76, 175, 80, 100), 2));
            painter.drawRect(cell);
            painter.restore();
        }
    }
}