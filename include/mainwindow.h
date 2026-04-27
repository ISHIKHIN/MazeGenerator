#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QFrame>
#include <QTimer>
#include <vector>
#include <memory>
#include "difficultysettings.h"
#include "mazegenerator.h"
#include "pathfinder.h"

class MazeWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void generateMaze();
    void findPath();
    void clearPath();
    void updateSizeByDifficulty();
    void onMazeGenerated();
    void adjustMazeSize();

private:
    void setupUI();
    void setupConnections();
    void updateMazeWidgetSize();

    MazeWidget *mazeWidget;
    QScrollArea *scrollArea;
    QComboBox *algoBox;
    QComboBox *pathBox;
    QComboBox *difficultyBox;
    QSpinBox *widthSpin;
    QSpinBox *heightSpin;
    QPushButton *genButton;
    QPushButton *pathButton;
    QPushButton *clearPathButton;
    QCheckBox *animateGeneration;
    QCheckBox *animatePath;
    QLabel *statusLabel;

    std::vector<std::vector<bool>> currentMaze;
    std::vector<std::pair<int, int>> currentPath;
    GeneratorSettings settings;

    std::unique_ptr<IMazeGenerator> mazeGenerator;
    std::unique_ptr<IPathFinder> pathFinder;
};

#endif