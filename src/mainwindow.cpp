#include "mainwindow.h"
#include "mazewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QLabel>
#include <QSpinBox>
#include <QScrollArea>
#include <QApplication>
#include <QScreen>
#include <QFrame>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setupConnections();

    mazeGenerator = std::make_unique<DFSGenerator>();
    pathFinder = std::make_unique<BFSFinder>();

    generateMaze();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    setWindowTitle("Генератор лабиринтов - C++/Qt6");

    auto *central = new QWidget;
    auto *mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    auto *mazeContainer = new QWidget;
    auto *mazeLayout = new QVBoxLayout(mazeContainer);
    mazeLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet(
        "QScrollArea { border: 2px solid #cccccc; background-color: #f0f0f0; border-radius: 5px; }"
        );

    mazeWidget = new MazeWidget;
    scrollArea->setWidget(mazeWidget);
    mazeLayout->addWidget(scrollArea);

    auto *controlPanel = new QFrame;
    controlPanel->setFrameStyle(QFrame::StyledPanel);
    controlPanel->setMaximumWidth(350);
    controlPanel->setMinimumWidth(300);
    auto *controlsLayout = new QVBoxLayout(controlPanel);
    controlsLayout->setSpacing(15);

    auto *titleLabel = new QLabel("Управление лабиринтом");
    titleLabel->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: #333; padding: 5px; background-color: white;"
        );
    controlsLayout->addWidget(titleLabel);

    auto *genGroup = new QGroupBox("Генерация (DFS / Крускал)");
    genGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #ddd; border-radius: 8px; "
        "margin-top: 10px; padding-top: 10px; background-color: white; color: black; } "
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }"
        );
    auto *genLayout = new QVBoxLayout;
    genLayout->setSpacing(10);

    auto *algoLayout = new QHBoxLayout;
    auto *algoLabel = new QLabel("Алгоритм:");
    algoLabel->setMinimumWidth(80);
    algoLabel->setStyleSheet("color: black;");
    algoLayout->addWidget(algoLabel);
    algoBox = new QComboBox;
    algoBox->addItems({"DFS (Поиск в глубину)", "Крускал"});
    algoBox->setStyleSheet("QComboBox { padding: 5px; border: 1px solid #ddd; border-radius: 4px; background-color: white; color: black; }");
    algoLayout->addWidget(algoBox);
    genLayout->addLayout(algoLayout);

    auto *diffLayout = new QHBoxLayout;
    auto *diffLabel = new QLabel("Сложность:");
    diffLabel->setMinimumWidth(80);
    diffLabel->setStyleSheet("color: black;");
    diffLayout->addWidget(diffLabel);
    difficultyBox = new QComboBox;
    difficultyBox->addItems({"Лёгкая (15x11)", "Средняя (21x15)", "Сложная (31x21)"});
    difficultyBox->setStyleSheet("QComboBox { padding: 5px; border: 1px solid #ddd; border-radius: 4px; background-color: white; color: black; }");
    diffLayout->addWidget(difficultyBox);
    genLayout->addLayout(diffLayout);

    auto *separator1 = new QFrame;
    separator1->setFrameShape(QFrame::HLine);
    separator1->setFrameShadow(QFrame::Sunken);
    genLayout->addWidget(separator1);

    auto *sizeLabel = new QLabel("Пользовательский размер:");
    sizeLabel->setStyleSheet("font-weight: bold; margin-top: 5px; color: black;");
    genLayout->addWidget(sizeLabel);

    auto *sizeLayout = new QHBoxLayout;
    auto *widthLabel = new QLabel("Ширина:");
    widthLabel->setMinimumWidth(50);
    widthLabel->setStyleSheet("color: black;");
    sizeLayout->addWidget(widthLabel);
    widthSpin = new QSpinBox;
    widthSpin->setRange(5, 100);
    widthSpin->setValue(21);
    widthSpin->setToolTip("Ширина лабиринта в ячейках (5-100)");
    widthSpin->setStyleSheet("QSpinBox { padding: 5px; border: 1px solid #ddd; border-radius: 4px; background-color: white; color: black; }");
    sizeLayout->addWidget(widthSpin);
    sizeLayout->addSpacing(10);

    auto *heightLabel = new QLabel("Высота:");
    heightLabel->setMinimumWidth(50);
    heightLabel->setStyleSheet("color: black;");
    sizeLayout->addWidget(heightLabel);
    heightSpin = new QSpinBox;
    heightSpin->setRange(5, 100);
    heightSpin->setValue(15);
    heightSpin->setToolTip("Высота лабиринта в ячейках (5-100)");
    heightSpin->setStyleSheet("QSpinBox { padding: 5px; border: 1px solid #ddd; border-radius: 4px; background-color: white; color: black; }");
    sizeLayout->addWidget(heightSpin);
    genLayout->addLayout(sizeLayout);

    animateGeneration = new QCheckBox("Анимация генерации");
    animateGeneration->setStyleSheet("QCheckBox { spacing: 8px; color: black; }");
    genLayout->addWidget(animateGeneration);

    genButton = new QPushButton("Сгенерировать");
    genButton->setMinimumHeight(40);
    genButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; font-weight: bold; "
        "border: none; border-radius: 5px; } "
        "QPushButton:hover { background-color: #45a049; } "
        "QPushButton:pressed { background-color: #3d8b40; }"
        );
    genLayout->addWidget(genButton);

    genGroup->setLayout(genLayout);

    auto *pathGroup = new QGroupBox("Поиск пути (BFS / A*)");
    pathGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #ddd; border-radius: 8px; "
        "margin-top: 10px; padding-top: 10px; background-color: white; color: black; } "
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }"
        );
    auto *pathLayout = new QVBoxLayout;
    pathLayout->setSpacing(10);

    auto *pathAlgoLayout = new QHBoxLayout;
    auto *pathAlgoLabel = new QLabel("Алгоритм:");
    pathAlgoLabel->setMinimumWidth(80);
    pathAlgoLabel->setStyleSheet("color: black;");
    pathAlgoLayout->addWidget(pathAlgoLabel);
    pathBox = new QComboBox;
    pathBox->addItems({"BFS (Поиск в ширину)", "A* (Звездочка)"});
    pathBox->setStyleSheet("QComboBox { padding: 5px; border: 1px solid #ddd; border-radius: 4px; background-color: white; color: black; }");
    pathAlgoLayout->addWidget(pathBox);
    pathLayout->addLayout(pathAlgoLayout);

    animatePath = new QCheckBox("Анимация поиска");
    animatePath->setStyleSheet("QCheckBox { spacing: 8px; color: black; }");
    pathLayout->addWidget(animatePath);

    pathButton = new QPushButton("Найти путь");
    pathButton->setMinimumHeight(35);
    pathButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; font-weight: bold; "
        "border: none; border-radius: 5px; } "
        "QPushButton:hover { background-color: #0b7dda; } "
        "QPushButton:pressed { background-color: #0a6ebd; }"
        );
    pathLayout->addWidget(pathButton);

    clearPathButton = new QPushButton("Убрать путь");
    clearPathButton->setMinimumHeight(35);
    clearPathButton->setStyleSheet(
        "QPushButton { background-color: #FF9800; color: white; font-weight: bold; "
        "border: none; border-radius: 5px; } "
        "QPushButton:hover { background-color: #fb8c00; } "
        "QPushButton:pressed { background-color: #f57c00; }"
        );
    pathLayout->addWidget(clearPathButton);

    pathGroup->setLayout(pathLayout);

    statusLabel = new QLabel("Готов к работе");
    statusLabel->setStyleSheet(
        "QLabel { background-color: #e8f5e9; padding: 8px; border-radius: 5px; "
        "color: #2e7d32; font-weight: bold; }"
        );
    statusLabel->setWordWrap(true);

    controlsLayout->addWidget(genGroup);
    controlsLayout->addWidget(pathGroup);
    controlsLayout->addWidget(statusLabel);
    controlsLayout->addStretch();

    mainLayout->addWidget(mazeContainer, 2);
    mainLayout->addWidget(controlPanel, 1);

    setCentralWidget(central);

    QRect screenGeometry;
    if (QScreen *screen = QApplication::primaryScreen()) {
        screenGeometry = screen->geometry();
    } else {
        screenGeometry = QRect(0, 0, 1280, 720);
    }

    int windowWidth = qMin(1400, screenGeometry.width() - 100);
    int windowHeight = qMin(900, screenGeometry.height() - 100);
    resize(windowWidth, windowHeight);
}

void MainWindow::setupConnections() {
    connect(genButton, &QPushButton::clicked, this, &MainWindow::generateMaze);
    connect(pathButton, &QPushButton::clicked, this, &MainWindow::findPath);
    connect(clearPathButton, &QPushButton::clicked, this, &MainWindow::clearPath);
    connect(difficultyBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateSizeByDifficulty);

    connect(algoBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                if (index == 0) {
                    mazeGenerator = std::make_unique<DFSGenerator>();
                    statusLabel->setText("Стратегия генерации: DFS");
                } else {
                    mazeGenerator = std::make_unique<KruskalGenerator>();
                    statusLabel->setText("Стратегия генерации: Крускал");
                }
            });

    connect(pathBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                if (index == 0) {
                    pathFinder = std::make_unique<BFSFinder>();
                    statusLabel->setText("Стратегия поиска: BFS");
                } else {
                    pathFinder = std::make_unique<AStarFinder>();
                    statusLabel->setText("Стратегия поиска: A*");
                }
            });
}

void MainWindow::updateSizeByDifficulty() {
    Difficulty diff;
    switch(difficultyBox->currentIndex()) {
    case 0: diff = EASY; break;
    case 1: diff = MEDIUM; break;
    default: diff = HARD; break;
    }
    settings.setDifficulty(diff);
    widthSpin->setValue(settings.width);
    heightSpin->setValue(settings.height);
}

void MainWindow::updateMazeWidgetSize() {
    adjustMazeSize();
}

void MainWindow::adjustMazeSize() {
    if (mazeWidget && scrollArea) {
        QSize availableSize = scrollArea->viewport()->size();
        if (availableSize.width() > 0 && availableSize.height() > 0) {
            mazeWidget->updateSize(availableSize);
        }
    }
}

void MainWindow::generateMaze() {
    if (!mazeGenerator) {
        statusLabel->setText("Ошибка: стратегия генерации не инициализирована");
        return;
    }

    try {
        settings.width = widthSpin->value();
        settings.height = heightSpin->value();

        if (settings.width * settings.height > 5000) {
            QMessageBox::StandardButton reply = QMessageBox::warning(
                this, "Предупреждение",
                "Очень большой лабиринт может работать медленно.\n"
                "Рекомендуемый размер: до 100x100 ячеек.\n"
                "Продолжить генерацию?",
                QMessageBox::Yes | QMessageBox::No
                );
            if (reply == QMessageBox::No) {
                return;
            }
        }

        statusLabel->setText(QString("Генерация лабиринта (стратегия: %1)...")
                                 .arg(mazeGenerator->name()));
        QApplication::processEvents();

        currentMaze = mazeGenerator->generate(settings.width, settings.height);

        currentPath.clear();
        mazeWidget->clearPath();

        mazeWidget->setMaze(currentMaze, animateGeneration->isChecked());
        adjustMazeSize();

        statusLabel->setText(QString("Лабиринт %1x%2 успешно сгенерирован (стратегия: %3)")
                                 .arg(settings.width).arg(settings.height)
                                 .arg(mazeGenerator->name()));

    } catch (const std::exception& e) {
        statusLabel->setText("Ошибка генерации лабиринта");
        QMessageBox::critical(this, "Ошибка",
                              QString("Не удалось сгенерировать лабиринт:\n%1").arg(e.what()));
    }
}

void MainWindow::findPath() {
    if (!pathFinder) {
        statusLabel->setText("Ошибка: стратегия поиска не инициализирована");
        return;
    }

    if (currentMaze.empty()) {
        QMessageBox::warning(this, "Предупреждение", "Сначала сгенерируйте лабиринт!");
        return;
    }

    int rows = (int)currentMaze.size();
    int cols = (int)currentMaze[0].size();

    std::pair<int, int> start = {1, 1};
    std::pair<int, int> end = {cols - 2, rows - 2};

    statusLabel->setText(QString("Поиск пути (стратегия: %1)...")
                             .arg(pathFinder->name()));
    QApplication::processEvents();

    currentPath = pathFinder->findPath(currentMaze, start, end);

    if (currentPath.empty()) {
        statusLabel->setText("Путь не найден!");
        QMessageBox::information(this, "Путь не найден",
                                 "Путь от старта до финиша в этом лабиринте не существует!\n"
                                 "Попробуйте сгенерировать другой лабиринт.");
    } else {
        mazeWidget->setPath(currentPath, animatePath->isChecked());

        statusLabel->setText(QString("Путь найден! Длина: %1 шагов (стратегия: %2)")
                                 .arg(currentPath.size()).arg(pathFinder->name()));
    }
}

void MainWindow::clearPath() {
    currentPath.clear();
    mazeWidget->clearPath();
    statusLabel->setText("Путь удален");
}

void MainWindow::onMazeGenerated() {
    adjustMazeSize();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    QTimer::singleShot(100, this, &MainWindow::adjustMazeSize);
}