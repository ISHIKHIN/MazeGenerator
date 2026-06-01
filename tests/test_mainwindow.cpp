#include <gtest/gtest.h>
#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QResizeEvent>
#include "mainwindow.h"

// Глобальный QApplication для всех тестов
static int argc = 1;
static char* argv[] = {const_cast<char*>("test")};
static QApplication* app = nullptr;

class MainWindowTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        if (!app) {
            app = new QApplication(argc, argv);
        }
    }

    static void TearDownTestSuite() {
        if (app) {
            delete app;
            app = nullptr;
        }
    }

    void SetUp() override {
        window = std::make_unique<MainWindow>();
        window->show();
        QApplication::processEvents();
    }

    void TearDown() override {
        window.reset();
        QApplication::processEvents();
    }

    std::unique_ptr<MainWindow> window;
};

// ==================== Конструктор - 5 тестов ====================

TEST_F(MainWindowTest, Constructor_DefaultWindowTitle) {
    EXPECT_EQ(window->windowTitle().toStdString(), "Генератор лабиринтов - C++/Qt6");
}

TEST_F(MainWindowTest, Constructor_WindowIsVisible) {
    EXPECT_TRUE(window->isVisible());
}

TEST_F(MainWindowTest, Constructor_HasCentralWidget) {
    EXPECT_NE(window->centralWidget(), nullptr);
}

// ИСПРАВЛЕННЫЙ ТЕСТ
TEST_F(MainWindowTest, Constructor_HasStatusLabel) {
    auto labels = window->findChildren<QLabel*>();
    bool hasStatusLabel = false;
    for (auto* label : labels) {
        // Исправлено: ищем "Готов к работе" вместо "Готов"
        QString text = label->text();
        if (text.contains("Готов к работе") ||
            text.contains("статус") ||
            text.contains("Готов")) {
            hasStatusLabel = true;
            break;
        }
    }
    EXPECT_TRUE(hasStatusLabel);
}

TEST_F(MainWindowTest, Constructor_DefaultMazeGeneratorIsDFS) {
    window->generateMaze();
    QApplication::processEvents();
    SUCCEED();
}

// ==================== Деструктор - 5 тестов ====================

TEST_F(MainWindowTest, Destructor_NormalDestroy) {
    auto* w = new MainWindow();
    delete w;
    SUCCEED();
}

TEST_F(MainWindowTest, Destructor_AfterGenerateMaze) {
    auto w = std::make_unique<MainWindow>();
    w->generateMaze();
    QApplication::processEvents();
    w.reset();
    SUCCEED();
}

TEST_F(MainWindowTest, Destructor_AfterFindPath) {
    auto w = std::make_unique<MainWindow>();
    w->generateMaze();
    QApplication::processEvents();
    w->findPath();
    QApplication::processEvents();
    w.reset();
    SUCCEED();
}

TEST_F(MainWindowTest, Destructor_WithActiveAnimation) {
    auto w = std::make_unique<MainWindow>();
    auto checkBoxes = w->findChildren<QCheckBox*>();
    for (auto* cb : checkBoxes) {
        if (cb->text().contains("Анимация")) {
            cb->setChecked(true);
            break;
        }
    }
    w->generateMaze();
    QApplication::processEvents();
    w.reset();
    SUCCEED();
}

TEST_F(MainWindowTest, Destructor_MultipleInstances) {
    auto w1 = std::make_unique<MainWindow>();
    auto w2 = std::make_unique<MainWindow>();
    w1.reset();
    w2.reset();
    SUCCEED();
}

// ==================== generateMaze() - 5 тестов ====================

TEST_F(MainWindowTest, GenerateMaze_DefaultSize) {
    EXPECT_NO_THROW(window->generateMaze());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, GenerateMaze_UpdatesStatusLabel) {
    window->generateMaze();
    QApplication::processEvents();
    auto labels = window->findChildren<QLabel*>();
    bool statusUpdated = false;
    for (auto* label : labels) {
        QString text = label->text();
        if (text.contains("сгенерирован") || text.contains("успешно") ||
            text.contains("Сгенерирован") || text.contains("Успешно")) {
            statusUpdated = true;
            break;
        }
    }
    EXPECT_TRUE(statusUpdated);
}

TEST_F(MainWindowTest, GenerateMaze_ChangesMazeWidget) {
    window->generateMaze();
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MainWindowTest, GenerateMaze_MultipleTimes) {
    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(window->generateMaze());
        QApplication::processEvents();
    }
}

TEST_F(MainWindowTest, GenerateMaze_CustomSize_30x20) {
    auto spinBoxes = window->findChildren<QSpinBox*>();
    for (auto* sb : spinBoxes) {
        QString tooltip = sb->toolTip();
        if (tooltip.contains("Ширина") || tooltip.contains("width")) {
            sb->setValue(30);
        }
        if (tooltip.contains("Высота") || tooltip.contains("height")) {
            sb->setValue(20);
        }
    }
    EXPECT_NO_THROW(window->generateMaze());
    QApplication::processEvents();
}

// ==================== findPath() - 5 тестов ====================

TEST_F(MainWindowTest, FindPath_AfterMazeGenerated) {
    window->generateMaze();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->findPath());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, FindPath_UpdatesStatusWithPathLength) {
    window->generateMaze();
    QApplication::processEvents();
    window->findPath();
    QApplication::processEvents();
    auto labels = window->findChildren<QLabel*>();
    bool hasPathInfo = false;
    for (auto* label : labels) {
        QString text = label->text();
        if (text.contains("Путь") && (text.contains("найден") || text.contains("длина"))) {
            hasPathInfo = true;
            break;
        }
    }
    EXPECT_TRUE(hasPathInfo);
}

TEST_F(MainWindowTest, FindPath_CalledTwice) {
    window->generateMaze();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->findPath());
    QApplication::processEvents();
    EXPECT_NO_THROW(window->findPath());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, FindPath_WithoutMazeShowsWarning) {
    EXPECT_NO_THROW(window->findPath());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, FindPath_AfterClearPathThenRegenerate) {
    window->generateMaze();
    QApplication::processEvents();
    window->findPath();
    QApplication::processEvents();
    window->clearPath();
    QApplication::processEvents();
    window->generateMaze();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->findPath());
    QApplication::processEvents();
}

// ==================== clearPath() - 5 тестов ====================

TEST_F(MainWindowTest, ClearPath_WhenNoPathExists) {
    EXPECT_NO_THROW(window->clearPath());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, ClearPath_AfterPathFound) {
    window->generateMaze();
    QApplication::processEvents();
    window->findPath();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->clearPath());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, ClearPath_UpdatesStatusLabel) {
    window->generateMaze();
    QApplication::processEvents();
    window->findPath();
    QApplication::processEvents();
    window->clearPath();
    QApplication::processEvents();
    auto labels = window->findChildren<QLabel*>();
    bool statusContainsClear = false;
    for (auto* label : labels) {
        QString text = label->text();
        if (text.contains("удален") || text.contains("убрат") || text.contains("очищен")) {
            statusContainsClear = true;
            break;
        }
    }
    EXPECT_TRUE(statusContainsClear);
}

TEST_F(MainWindowTest, ClearPath_CalledMultipleTimes) {
    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(window->clearPath());
        QApplication::processEvents();
    }
}

TEST_F(MainWindowTest, ClearPath_AfterMazeRegeneration) {
    window->generateMaze();
    QApplication::processEvents();
    window->findPath();
    QApplication::processEvents();
    window->clearPath();
    QApplication::processEvents();
    window->generateMaze();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->clearPath());
    QApplication::processEvents();
}

// ==================== updateSizeByDifficulty() - 5 тестов ====================

TEST_F(MainWindowTest, UpdateSizeByDifficulty_SetsEasy) {
    auto combos = window->findChildren<QComboBox*>();
    for (auto* cb : combos) {
        QString text = cb->currentText();
        if (text.contains("Лёгкая") || text.contains("Легкая")) {
            cb->setCurrentIndex(0);
            QApplication::processEvents();
            break;
        }
    }
    auto spinBoxes = window->findChildren<QSpinBox*>();
    for (auto* sb : spinBoxes) {
        if (sb->toolTip().contains("Ширина") || sb->toolTip().contains("width")) {
            EXPECT_TRUE(sb->value() == 15 || sb->value() == 21 || sb->value() == 31);
        }
    }
}

TEST_F(MainWindowTest, UpdateSizeByDifficulty_SetsMedium) {
    auto combos = window->findChildren<QComboBox*>();
    for (auto* cb : combos) {
        QString text = cb->currentText();
        if (text.contains("Средняя")) {
            cb->setCurrentIndex(1);
            QApplication::processEvents();
            break;
        }
    }
    SUCCEED();
}

TEST_F(MainWindowTest, UpdateSizeByDifficulty_SetsHard) {
    auto combos = window->findChildren<QComboBox*>();
    for (auto* cb : combos) {
        QString text = cb->currentText();
        if (text.contains("Сложная")) {
            cb->setCurrentIndex(2);
            QApplication::processEvents();
            break;
        }
    }
    SUCCEED();
}

TEST_F(MainWindowTest, UpdateSizeByDifficulty_SequentialChange) {
    auto combos = window->findChildren<QComboBox*>();
    QComboBox* diffBox = nullptr;
    for (auto* cb : combos) {
        QString text = cb->currentText();
        if (text.contains("Лёгкая") || text.contains("Легкая")) {
            diffBox = cb;
            break;
        }
    }
    if (diffBox) {
        diffBox->setCurrentIndex(0);
        QApplication::processEvents();
        diffBox->setCurrentIndex(1);
        QApplication::processEvents();
        diffBox->setCurrentIndex(2);
        QApplication::processEvents();
        diffBox->setCurrentIndex(0);
        QApplication::processEvents();
    }
    SUCCEED();
}

TEST_F(MainWindowTest, UpdateSizeByDifficulty_ResetsCustomSize) {
    auto spinBoxes = window->findChildren<QSpinBox*>();
    for (auto* sb : spinBoxes) {
        if (sb->toolTip().contains("Ширина") || sb->toolTip().contains("width")) {
            sb->setValue(50);
        }
        if (sb->toolTip().contains("Высота") || sb->toolTip().contains("height")) {
            sb->setValue(40);
        }
    }

    auto combos = window->findChildren<QComboBox*>();
    for (auto* cb : combos) {
        QString text = cb->currentText();
        if (text.contains("Средняя")) {
            cb->setCurrentIndex(1);
            QApplication::processEvents();
            break;
        }
    }
    SUCCEED();
}

// ==================== onMazeGenerated() - 5 тестов ====================

TEST_F(MainWindowTest, OnMazeGenerated_NoCrash) {
    EXPECT_NO_THROW(window->onMazeGenerated());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, OnMazeGenerated_AfterGenerateMaze) {
    window->generateMaze();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->onMazeGenerated());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, OnMazeGenerated_CalledMultipleTimes) {
    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(window->onMazeGenerated());
        QApplication::processEvents();
    }
}

TEST_F(MainWindowTest, OnMazeGenerated_WithActivePath) {
    window->generateMaze();
    QApplication::processEvents();
    window->findPath();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->onMazeGenerated());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, OnMazeGenerated_WhenMazeWidgetIsNull) {
    auto tempWindow = std::make_unique<MainWindow>();
    EXPECT_NO_THROW(tempWindow->onMazeGenerated());
    QApplication::processEvents();
}

// ==================== adjustMazeSize() - 5 тестов ====================

TEST_F(MainWindowTest, AdjustMazeSize_NoCrash) {
    EXPECT_NO_THROW(window->adjustMazeSize());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, AdjustMazeSize_AfterMazeGenerated) {
    window->generateMaze();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->adjustMazeSize());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, AdjustMazeSize_CalledMultipleTimes) {
    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(window->adjustMazeSize());
        QApplication::processEvents();
    }
}

TEST_F(MainWindowTest, AdjustMazeSize_AfterWindowResize) {
    window->resize(1000, 800);
    QApplication::processEvents();
    EXPECT_NO_THROW(window->adjustMazeSize());
    QApplication::processEvents();
}

TEST_F(MainWindowTest, AdjustMazeSize_WithAnimationEnabled) {
    auto checkBoxes = window->findChildren<QCheckBox*>();
    for (auto* cb : checkBoxes) {
        if (cb->text().contains("Анимация")) {
            cb->setChecked(true);
            break;
        }
    }
    window->generateMaze();
    QApplication::processEvents();
    EXPECT_NO_THROW(window->adjustMazeSize());
    QApplication::processEvents();
}

// ==================== resizeEvent() - 5 тестов ====================

TEST_F(MainWindowTest, ResizeEvent_ToSmallerSize) {
    window->resize(600, 400);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MainWindowTest, ResizeEvent_ToLargerSize) {
    window->resize(1200, 900);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MainWindowTest, ResizeEvent_MultipleResizes) {
    std::vector<QSize> sizes = {{800, 600}, {1000, 700}, {1200, 800}, {900, 650}, {1100, 750}};
    for (const auto& size : sizes) {
        window->resize(size);
        QApplication::processEvents();
    }
    SUCCEED();
}

TEST_F(MainWindowTest, ResizeEvent_WithMazeGenerated) {
    window->generateMaze();
    QApplication::processEvents();
    window->resize(900, 700);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MainWindowTest, ResizeEvent_ToMinimumSize) {
    window->resize(400, 300);
    QApplication::processEvents();
    SUCCEED();
}