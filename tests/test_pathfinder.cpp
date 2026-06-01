#include <gtest/gtest.h>
#include "pathfinder.h"
#include <chrono>

class PathFinderTest : public ::testing::Test {
protected:
    void SetUp() override {
        bfsFinder = std::make_unique<BFSFinder>();
        astarFinder = std::make_unique<AStarFinder>();
    }

    std::unique_ptr<BFSFinder> bfsFinder;
    std::unique_ptr<AStarFinder> astarFinder;

    // Пустой лабиринт (все клетки проходимы)
    std::vector<std::vector<bool>> createEmptyMaze(int rows, int cols) {
        return std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false));
    }

    // Лабиринт с вертикальной стеной (есть обход слева и справа)
    std::vector<std::vector<bool>> createMazeWithVerticalWall(int rows, int cols, int wallCol) {
        auto maze = createEmptyMaze(rows, cols);
        // Создаем стену, но оставляем проходы сверху и снизу для обхода
        for (int i = 1; i < rows - 1; ++i) {
            maze[i][wallCol] = true;
        }
        return maze;
    }

    // Лабиринт с горизонтальной стеной (есть обход слева и справа)
    std::vector<std::vector<bool>> createMazeWithHorizontalWall(int rows, int cols, int wallRow) {
        auto maze = createEmptyMaze(rows, cols);
        // Создаем стену, но оставляем проходы слева и справа для обхода
        for (int j = 1; j < cols - 1; ++j) {
            maze[wallRow][j] = true;
        }
        return maze;
    }

    // Лабиринт с лабиринтом-стеной (полностью блокирует)
    std::vector<std::vector<bool>> createBlockedMaze(int rows, int cols) {
        auto maze = createEmptyMaze(rows, cols);
        // Создаем полную стену по всей высоте
        for (int i = 0; i < rows; ++i) {
            maze[i][cols / 2] = true;
        }
        return maze;
    }

    bool isValidPath(const std::vector<std::pair<int, int>>& path,
                     const std::vector<std::vector<bool>>& maze,
                     std::pair<int, int> start,
                     std::pair<int, int> end) {
        if (path.empty()) return false;
        if (path.front() != start) return false;
        if (path.back() != end) return false;

        for (size_t i = 1; i < path.size(); ++i) {
            int dx = std::abs(path[i].first - path[i-1].first);
            int dy = std::abs(path[i].second - path[i-1].second);
            if (dx + dy != 1) return false;
            if (path[i].second < 0 || path[i].second >= (int)maze.size() ||
                path[i].first < 0 || path[i].first >= (int)maze[0].size()) {
                return false;
            }
            if (maze[path[i].second][path[i].first]) return false;
        }
        return true;
    }
};

// ==================== BFS Tests ====================

TEST_F(PathFinderTest, BFSFindPathInEmptyMaze) {
    auto maze = createEmptyMaze(10, 10);
    auto path = bfsFinder->findPath(maze, {0, 0}, {9, 9});
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), std::make_pair(0, 0));
    EXPECT_EQ(path.back(), std::make_pair(9, 9));
}

TEST_F(PathFinderTest, BFSFindPathWithVerticalWall) {
    auto maze = createMazeWithVerticalWall(15, 15, 7);
    auto path = bfsFinder->findPath(maze, {0, 0}, {14, 14});
    // Должен найти путь, обходя стену слева
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), std::make_pair(0, 0));
    EXPECT_EQ(path.back(), std::make_pair(14, 14));
}

TEST_F(PathFinderTest, BFSFindPathWithHorizontalWall) {
    auto maze = createMazeWithHorizontalWall(15, 15, 7);
    auto path = bfsFinder->findPath(maze, {0, 0}, {14, 14});
    // Должен найти путь, обходя стену сверху
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), std::make_pair(0, 0));
    EXPECT_EQ(path.back(), std::make_pair(14, 14));
}

TEST_F(PathFinderTest, BFSUnreachablePath) {
    auto maze = createBlockedMaze(10, 10);
    auto path = bfsFinder->findPath(maze, {0, 0}, {9, 9});
    // Стена от верха до низа - пути быть не должно
    EXPECT_TRUE(path.empty());
}

TEST_F(PathFinderTest, BFSStartEqualsEnd) {
    auto maze = createEmptyMaze(5, 5);
    auto path = bfsFinder->findPath(maze, {2, 2}, {2, 2});
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path.front(), std::make_pair(2, 2));
}

TEST_F(PathFinderTest, BFSOneByOneMaze) {
    auto maze = std::vector<std::vector<bool>>(1, std::vector<bool>(1, false));
    auto path = bfsFinder->findPath(maze, {0, 0}, {0, 0});
    EXPECT_EQ(path.size(), 1);
}

TEST_F(PathFinderTest, BFSFinderName) {
    EXPECT_EQ(bfsFinder->name(), "BFS (Поиск в ширину)");
}

// ==================== A* Tests ====================

TEST_F(PathFinderTest, AStarFindPathInEmptyMaze) {
    auto maze = createEmptyMaze(15, 15);
    auto path = astarFinder->findPath(maze, {0, 0}, {14, 14});
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), std::make_pair(0, 0));
    EXPECT_EQ(path.back(), std::make_pair(14, 14));
}

TEST_F(PathFinderTest, AStarFindPathWithObstacles) {
    auto maze = createMazeWithVerticalWall(10, 10, 4);
    auto path = astarFinder->findPath(maze, {0, 0}, {9, 9});
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), std::make_pair(0, 0));
    EXPECT_EQ(path.back(), std::make_pair(9, 9));
}

TEST_F(PathFinderTest, AStarUnreachablePath) {
    auto maze = createBlockedMaze(10, 10);
    auto path = astarFinder->findPath(maze, {0, 0}, {9, 9});
    EXPECT_TRUE(path.empty());
}

TEST_F(PathFinderTest, AStarStartEqualsEnd) {
    auto maze = createEmptyMaze(5, 5);
    auto path = astarFinder->findPath(maze, {3, 3}, {3, 3});
    EXPECT_EQ(path.size(), 1);
}

TEST_F(PathFinderTest, AStarFinderName) {
    EXPECT_EQ(astarFinder->name(), "A* (Звездочка)");
}

TEST_F(PathFinderTest, AStarFindsOptimalPath) {
    auto maze = createEmptyMaze(10, 10);
    auto path = astarFinder->findPath(maze, {0, 0}, {5, 5});
    int optimalLength = std::abs(5-0) + std::abs(5-0) + 1;
    EXPECT_EQ(path.size(), optimalLength);
}

TEST_F(PathFinderTest, CompareBFSAndAStar) {
    auto maze = createEmptyMaze(20, 20);
    auto bfsPath = bfsFinder->findPath(maze, {0, 0}, {19, 19});
    auto astarPath = astarFinder->findPath(maze, {0, 0}, {19, 19});
    EXPECT_FALSE(bfsPath.empty());
    EXPECT_FALSE(astarPath.empty());
    EXPECT_EQ(bfsPath.size(), astarPath.size());
}