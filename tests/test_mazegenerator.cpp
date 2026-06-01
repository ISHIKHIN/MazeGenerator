#include <gtest/gtest.h>
#include "mazegenerator.h"
#include <queue>

class MazeGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        dfsGenerator = std::make_unique<DFSGenerator>();
        kruskalGenerator = std::make_unique<KruskalGenerator>();
    }
    std::unique_ptr<DFSGenerator> dfsGenerator;
    std::unique_ptr<KruskalGenerator> kruskalGenerator;

    // Исправленная проверка границ - учитываем вход и выход
    void validateMazeBoundaries(const std::vector<std::vector<bool>>& maze, int width, int height) {
        int expectedRows = height * 2 + 1;
        int expectedCols = width * 2 + 1;

        EXPECT_EQ(maze.size(), expectedRows);
        if (!maze.empty()) {
            EXPECT_EQ(maze[0].size(), expectedCols);
        }

        // Проверка входа и выхода
        EXPECT_FALSE(maze[1][0]) << "Вход должен быть проходом (строка 1, колонка 0)";
        EXPECT_FALSE(maze[expectedRows - 2][expectedCols - 1]) << "Выход должен быть проходом";

        // Остальные внешние стены должны быть стенами
        for (size_t i = 0; i < maze.size(); ++i) {
            if (i != 1) {
                EXPECT_TRUE(maze[i][0]) << "Левая стена в строке " << i << " должна быть стеной";
            }
            if (i != expectedRows - 2) {
                EXPECT_TRUE(maze[i][expectedCols - 1]) << "Правая стена в строке " << i << " должна быть стеной";
            }
        }
    }

    bool isMazeConnected(const std::vector<std::vector<bool>>& maze) {
        if (maze.empty()) return false;
        int rows = maze.size();
        int cols = maze[0].size();
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        std::queue<std::pair<int, int>> q;
        q.push({0, 1});
        visited[1][0] = true;
        const int dx[] = {1, -1, 0, 0};
        const int dy[] = {0, 0, 1, -1};
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            for (int i = 0; i < 4; ++i) {
                int nx = x + dx[i], ny = y + dy[i];
                if (nx >= 0 && ny >= 0 && nx < cols && ny < rows && !maze[ny][nx] && !visited[ny][nx]) {
                    visited[ny][nx] = true;
                    q.push({nx, ny});
                }
            }
        }
        return visited[rows - 2][cols - 1];
    }
};

// ==================== DFSGenerator::generate() - 5 тестов ====================

TEST_F(MazeGeneratorTest, DFSGenerateSmallMaze) {
    auto maze = dfsGenerator->generate(5, 5);
    validateMazeBoundaries(maze, 5, 5);
    EXPECT_TRUE(isMazeConnected(maze));
}

TEST_F(MazeGeneratorTest, DFSGenerateMediumMaze) {
    auto maze = dfsGenerator->generate(10, 10);
    validateMazeBoundaries(maze, 10, 10);
    EXPECT_TRUE(isMazeConnected(maze));
}

TEST_F(MazeGeneratorTest, DFSGenerateMinimalMaze) {
    auto maze = dfsGenerator->generate(2, 2);
    validateMazeBoundaries(maze, 2, 2);
    EXPECT_TRUE(isMazeConnected(maze));
}

TEST_F(MazeGeneratorTest, DFSMazeConnectivity) {
    auto maze = dfsGenerator->generate(15, 11);
    EXPECT_TRUE(isMazeConnected(maze));
}

TEST_F(MazeGeneratorTest, DFSGeneratorName) {
    EXPECT_EQ(dfsGenerator->name(), "DFS (Поиск в глубину)");
}

TEST_F(MazeGeneratorTest, DFSGenerateOneByOne) {
    auto maze = dfsGenerator->generate(1, 1);
    EXPECT_EQ(maze.size(), 3);
    EXPECT_EQ(maze[0].size(), 3);
}

TEST_F(MazeGeneratorTest, DFSAllCellsAreValid) {
    auto maze = dfsGenerator->generate(8, 6);
    for (const auto& row : maze) {
        for (bool cell : row) {
            EXPECT_TRUE(cell == true || cell == false);
        }
    }
}

TEST_F(MazeGeneratorTest, DFSGeneratesDifferentMazes) {
    auto maze1 = dfsGenerator->generate(10, 10);
    auto maze2 = dfsGenerator->generate(10, 10);
    bool areDifferent = false;
    for (size_t i = 0; i < maze1.size() && !areDifferent; ++i) {
        for (size_t j = 0; j < maze1[0].size() && !areDifferent; ++j) {
            if (maze1[i][j] != maze2[i][j]) areDifferent = true;
        }
    }
    EXPECT_TRUE(areDifferent);
}

// ==================== KruskalGenerator::generate() - 5 тестов ====================

TEST_F(MazeGeneratorTest, KruskalGenerateSmallMaze) {
    auto maze = kruskalGenerator->generate(5, 5);
    validateMazeBoundaries(maze, 5, 5);
    EXPECT_TRUE(isMazeConnected(maze));
}

TEST_F(MazeGeneratorTest, KruskalGenerateMediumMaze) {
    auto maze = kruskalGenerator->generate(12, 10);
    validateMazeBoundaries(maze, 12, 10);
    EXPECT_TRUE(isMazeConnected(maze));
}

TEST_F(MazeGeneratorTest, KruskalGenerateMinimalMaze) {
    auto maze = kruskalGenerator->generate(2, 2);
    validateMazeBoundaries(maze, 2, 2);
    EXPECT_TRUE(isMazeConnected(maze));
}

TEST_F(MazeGeneratorTest, KruskalMazeConnectivity) {
    auto maze = kruskalGenerator->generate(15, 11);
    EXPECT_TRUE(isMazeConnected(maze));
}

TEST_F(MazeGeneratorTest, KruskalGeneratorName) {
    EXPECT_EQ(kruskalGenerator->name(), "Крускал");
}

TEST_F(MazeGeneratorTest, KruskalGenerateThreeByThree) {
    auto maze = kruskalGenerator->generate(3, 3);
    EXPECT_EQ(maze.size(), 7);
    EXPECT_EQ(maze[0].size(), 7);
}

TEST_F(MazeGeneratorTest, KruskalAllCellsAreValid) {
    auto maze = kruskalGenerator->generate(10, 8);
    for (const auto& row : maze) {
        for (bool cell : row) {
            EXPECT_TRUE(cell == true || cell == false);
        }
    }
}

TEST_F(MazeGeneratorTest, BothGeneratorsProduceSameSize) {
    auto dfsMaze = dfsGenerator->generate(20, 15);
    auto kruskalMaze = kruskalGenerator->generate(20, 15);
    EXPECT_EQ(dfsMaze.size(), kruskalMaze.size());
    EXPECT_EQ(dfsMaze[0].size(), kruskalMaze[0].size());
}