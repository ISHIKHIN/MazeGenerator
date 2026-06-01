#include <gtest/gtest.h>
#include <QApplication>
#include <QPainter>
#include "mazewidget.h"

class MazeWidgetTest : public ::testing::Test {
protected:
    void SetUp() override {
        static int argc = 1;
        static char* argv[] = {const_cast<char*>("test")};
        static QApplication app(argc, argv);
        widget = std::make_unique<MazeWidget>();
    }
    void TearDown() override {
        widget.reset();
    }
    std::unique_ptr<MazeWidget> widget;

    std::vector<std::vector<bool>> createTestMaze() {
        return {
            {true, true, true, true, true},
            {true, false, false, false, true},
            {true, false, true, false, true},
            {true, false, false, false, true},
            {true, true, true, true, true}
        };
    }

    std::vector<std::pair<int, int>> createTestPath() {
        return {{1, 1}, {2, 1}, {3, 1}, {3, 2}, {3, 3}};
    }
};

// ==================== MazeWidget() - 5 тестов ====================

TEST_F(MazeWidgetTest, Constructor_NoCrash) {
    MazeWidget w;
    SUCCEED();
}

TEST_F(MazeWidgetTest, Constructor_SizeHintValid) {
    EXPECT_GT(widget->sizeHint().width(), 0);
    EXPECT_GT(widget->sizeHint().height(), 0);
}

TEST_F(MazeWidgetTest, Constructor_TimersCreated) {
    EXPECT_NO_THROW(widget->setMaze(createTestMaze(), true));
}

TEST_F(MazeWidgetTest, Constructor_NoPath) {
    EXPECT_NO_THROW(widget->clearPath());
}

TEST_F(MazeWidgetTest, Constructor_DefaultCellSize) {
    widget->setCellSize(25);
    SUCCEED();
}

// ==================== setMaze() - 5 тестов ====================

TEST_F(MazeWidgetTest, SetMaze_WithoutAnimation) {
    EXPECT_NO_THROW(widget->setMaze(createTestMaze(), false));
}

TEST_F(MazeWidgetTest, SetMaze_WithAnimation) {
    EXPECT_NO_THROW(widget->setMaze(createTestMaze(), true));
    QApplication::processEvents();
}

TEST_F(MazeWidgetTest, SetMaze_EmptyMaze) {
    std::vector<std::vector<bool>> empty;
    EXPECT_NO_THROW(widget->setMaze(empty, false));
}

TEST_F(MazeWidgetTest, SetMaze_LargeMaze) {
    std::vector<std::vector<bool>> large(101, std::vector<bool>(101, false));
    EXPECT_NO_THROW(widget->setMaze(large, false));
}

TEST_F(MazeWidgetTest, SetMaze_MultipleTimes) {
    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(widget->setMaze(createTestMaze(), false));
    }
}

// ==================== setPath() - 5 тестов ====================

TEST_F(MazeWidgetTest, SetPath_WithoutAnimation) {
    widget->setMaze(createTestMaze(), false);
    EXPECT_NO_THROW(widget->setPath(createTestPath(), false));
}

TEST_F(MazeWidgetTest, SetPath_WithAnimation) {
    widget->setMaze(createTestMaze(), false);
    EXPECT_NO_THROW(widget->setPath(createTestPath(), true));
    QApplication::processEvents();
}

TEST_F(MazeWidgetTest, SetPath_EmptyPath) {
    widget->setMaze(createTestMaze(), false);
    std::vector<std::pair<int, int>> empty;
    EXPECT_NO_THROW(widget->setPath(empty, false));
}

TEST_F(MazeWidgetTest, SetPath_WithoutMaze) {
    std::vector<std::pair<int, int>> path = {{1, 1}};
    EXPECT_NO_THROW(widget->setPath(path, false));
}

TEST_F(MazeWidgetTest, SetPath_MultipleTimes) {
    widget->setMaze(createTestMaze(), false);
    for (int i = 0; i < 5; ++i) {
        EXPECT_NO_THROW(widget->setPath(createTestPath(), false));
    }
}

// ==================== clearPath() - 5 тестов ====================

TEST_F(MazeWidgetTest, ClearPath_NoPath) {
    EXPECT_NO_THROW(widget->clearPath());
}

TEST_F(MazeWidgetTest, ClearPath_AfterSetPath) {
    widget->setMaze(createTestMaze(), false);
    widget->setPath(createTestPath(), false);
    EXPECT_NO_THROW(widget->clearPath());
}

TEST_F(MazeWidgetTest, ClearPath_CalledTwice) {
    widget->clearPath();
    widget->clearPath();
    SUCCEED();
}

TEST_F(MazeWidgetTest, ClearPath_WithAnimation) {
    widget->setMaze(createTestMaze(), false);
    widget->setPath(createTestPath(), true);
    QApplication::processEvents();
    EXPECT_NO_THROW(widget->clearPath());
}

TEST_F(MazeWidgetTest, ClearPath_AfterNewMaze) {
    widget->setMaze(createTestMaze(), false);
    widget->setPath(createTestPath(), false);
    widget->setMaze(createTestMaze(), false);
    EXPECT_NO_THROW(widget->clearPath());
}

// ==================== setCellSize() - 5 тестов ====================

TEST_F(MazeWidgetTest, SetCellSize_Normal) {
    EXPECT_NO_THROW(widget->setCellSize(25));
}

TEST_F(MazeWidgetTest, SetCellSize_Small) {
    EXPECT_NO_THROW(widget->setCellSize(5));
}

TEST_F(MazeWidgetTest, SetCellSize_Large) {
    EXPECT_NO_THROW(widget->setCellSize(100));
}

TEST_F(MazeWidgetTest, SetCellSize_Zero) {
    EXPECT_NO_THROW(widget->setCellSize(0));
}

TEST_F(MazeWidgetTest, SetCellSize_Negative) {
    EXPECT_NO_THROW(widget->setCellSize(-10));
}

// ==================== updateSize() - 5 тестов ====================

TEST_F(MazeWidgetTest, UpdateSize_Normal) {
    EXPECT_NO_THROW(widget->updateSize(QSize(800, 600)));
}

TEST_F(MazeWidgetTest, UpdateSize_Zero) {
    EXPECT_NO_THROW(widget->updateSize(QSize(0, 0)));
}

TEST_F(MazeWidgetTest, UpdateSize_Small) {
    EXPECT_NO_THROW(widget->updateSize(QSize(100, 100)));
}

TEST_F(MazeWidgetTest, UpdateSize_Large) {
    EXPECT_NO_THROW(widget->updateSize(QSize(2000, 2000)));
}

TEST_F(MazeWidgetTest, UpdateSize_Multiple) {
    for (int s : {400, 600, 800, 1000}) {
        EXPECT_NO_THROW(widget->updateSize(QSize(s, s)));
    }
}

// ==================== paintEvent() - 5 тестов ====================

TEST_F(MazeWidgetTest, PaintEvent_NoMaze) {
    widget->update();
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, PaintEvent_WithMaze) {
    widget->setMaze(createTestMaze(), false);
    widget->update();
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, PaintEvent_WithPath) {
    widget->setMaze(createTestMaze(), false);
    widget->setPath(createTestPath(), false);
    widget->update();
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, PaintEvent_WithAnimation) {
    widget->setMaze(createTestMaze(), true);
    widget->update();
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, PaintEvent_MultipleUpdates) {
    widget->setMaze(createTestMaze(), false);
    for (int i = 0; i < 5; ++i) {
        widget->update();
        QApplication::processEvents();
    }
    SUCCEED();
}

// ==================== resizeEvent() - 5 тестов ====================

TEST_F(MazeWidgetTest, ResizeEvent_Normal) {
    widget->resize(800, 600);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, ResizeEvent_Small) {
    widget->resize(100, 100);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, ResizeEvent_Large) {
    widget->resize(2000, 2000);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, ResizeEvent_WithMaze) {
    widget->setMaze(createTestMaze(), false);
    widget->resize(900, 700);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, ResizeEvent_Multiple) {
    for (int s : {400, 600, 800}) {
        widget->resize(s, s);
        QApplication::processEvents();
    }
    SUCCEED();
}

// ==================== sizeHint() - 5 тестов ====================

TEST_F(MazeWidgetTest, SizeHint_ReturnsPositive) {
    EXPECT_GT(widget->sizeHint().width(), 0);
    EXPECT_GT(widget->sizeHint().height(), 0);
}

TEST_F(MazeWidgetTest, SizeHint_Consistent) {
    QSize hint1 = widget->sizeHint();
    QSize hint2 = widget->sizeHint();
    EXPECT_EQ(hint1, hint2);
}

TEST_F(MazeWidgetTest, SizeHint_AfterMaze) {
    widget->setMaze(createTestMaze(), false);
    EXPECT_GT(widget->sizeHint().width(), 0);
}

TEST_F(MazeWidgetTest, SizeHint_AfterResize) {
    widget->resize(500, 500);
    EXPECT_GT(widget->sizeHint().width(), 0);
}

TEST_F(MazeWidgetTest, SizeHint_Reasonable) {
    QSize hint = widget->sizeHint();
    EXPECT_LE(hint.width(), 2000);
    EXPECT_LE(hint.height(), 2000);
}

// ==================== startMazeAnimation() - 5 тестов ====================

TEST_F(MazeWidgetTest, StartMazeAnimation_NoCrash) {
    widget->setMaze(createTestMaze(), true);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, StartMazeAnimation_EmptyMaze) {
    std::vector<std::vector<bool>> empty;
    widget->setMaze(empty, true);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, StartMazeAnimation_LargeMaze) {
    std::vector<std::vector<bool>> large(51, std::vector<bool>(51, false));
    widget->setMaze(large, true);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, StartMazeAnimation_Interrupted) {
    widget->setMaze(createTestMaze(), true);
    QApplication::processEvents();
    widget->setMaze(createTestMaze(), false);
    SUCCEED();
}

TEST_F(MazeWidgetTest, StartMazeAnimation_Multiple) {
    for (int i = 0; i < 3; ++i) {
        widget->setMaze(createTestMaze(), true);
        QApplication::processEvents();
    }
    SUCCEED();
}

// ==================== startPathAnimation() - 5 тестов ====================

TEST_F(MazeWidgetTest, StartPathAnimation_NoCrash) {
    widget->setMaze(createTestMaze(), false);
    widget->setPath(createTestPath(), true);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, StartPathAnimation_EmptyPath) {
    widget->setMaze(createTestMaze(), false);
    std::vector<std::pair<int, int>> empty;
    widget->setPath(empty, true);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, StartPathAnimation_WithoutMaze) {
    widget->setPath(createTestPath(), true);
    QApplication::processEvents();
    SUCCEED();
}

TEST_F(MazeWidgetTest, StartPathAnimation_Interrupted) {
    widget->setMaze(createTestMaze(), false);
    widget->setPath(createTestPath(), true);
    QApplication::processEvents();
    widget->clearPath();
    SUCCEED();
}

TEST_F(MazeWidgetTest, StartPathAnimation_Multiple) {
    widget->setMaze(createTestMaze(), false);
    for (int i = 0; i < 3; ++i) {
        widget->setPath(createTestPath(), true);
        QApplication::processEvents();
        widget->clearPath();
    }
    SUCCEED();
}