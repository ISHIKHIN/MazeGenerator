#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QSize>
#include <vector>
#include <utility>

class MazeWidget : public QWidget {
    Q_OBJECT

public:
    explicit MazeWidget(QWidget *parent = nullptr);

    void setMaze(const std::vector<std::vector<bool>>& maze, bool animate = false);
    void setPath(const std::vector<std::pair<int,int>>& path, bool animate = false);
    void clearPath();
    void setCellSize(int size) { cellSize = size; update(); }
    void updateSize(const QSize& availableSize);
    QSize sizeHint() const override { return QSize(800, 600); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void animateMazeStep();
    void animatePathStep();

private:
    void startMazeAnimation();
    void startPathAnimation();
    void calculateOptimalCellSize();

    std::vector<std::vector<bool>> m_maze;
    std::vector<std::vector<bool>> m_animatedMaze;
    std::vector<std::pair<int,int>> m_path;
    std::vector<std::pair<int,int>> m_animatedPath;
    int cellSize = 25;
    int animationStep = 0;
    QTimer *mazeTimer;
    QTimer *pathTimer;
    bool animatingMaze = false;
    bool animatingPath = false;
    QSize lastAvailableSize;
};

#endif