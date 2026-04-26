#include "maze/mazegenerator.h"
#include <stack>
#include <random>
#include <algorithm>
#include <numeric>
#include <tuple>

struct DSU {
    std::vector<int> parent;

    DSU(int n) : parent(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        parent[find(x)] = find(y);
    }
};

std::vector<std::vector<bool>> DFSGenerator::generate(int width, int height) {
    int rows = height * 2 + 1;
    int cols = width * 2 + 1;

    std::vector<std::vector<bool>> maze(rows, std::vector<bool>(cols, true));

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distX(0, width - 1);
    std::uniform_int_distribution<int> distY(0, height - 1);

    int startX = distX(rng);
    int startY = distY(rng);
    int sx = startX * 2 + 1;
    int sy = startY * 2 + 1;
    maze[sy][sx] = false;

    std::stack<std::pair<int, int>> st;
    st.push({sx, sy});

    const int dx[] = {2, -2, 0, 0};
    const int dy[] = {0, 0, 2, -2};

    while (!st.empty()) {
        auto [cx, cy] = st.top();

        std::vector<int> dirs = {0, 1, 2, 3};
        std::shuffle(dirs.begin(), dirs.end(), rng);

        bool moved = false;

        for (int d : dirs) {
            int nx = cx + dx[d];
            int ny = cy + dy[d];

            if (nx > 0 && nx < cols - 1 && ny > 0 && ny < rows - 1 && maze[ny][nx]) {
                maze[ny][nx] = false;
                maze[cy + dy[d] / 2][cx + dx[d] / 2] = false;
                st.push({nx, ny});
                moved = true;
                break;
            }
        }

        if (!moved) {
            st.pop();
        }
    }

    maze[1][0] = false;
    maze[rows - 2][cols - 1] = false;

    return maze;
}

std::vector<std::vector<bool>> KruskalGenerator::generate(int width, int height) {
    int rows = height * 2 + 1;
    int cols = width * 2 + 1;

    std::vector<std::vector<bool>> maze(rows, std::vector<bool>(cols, true));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            maze[y * 2 + 1][x * 2 + 1] = false;
        }
    }

    std::random_device rd;
    std::mt19937 rng(rd());

    std::vector<std::tuple<int, int, int, int>> walls;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x + 1 < width) {
                walls.push_back({x, y, x + 1, y});
            }
            if (y + 1 < height) {
                walls.push_back({x, y, x, y + 1});
            }
        }
    }

    std::shuffle(walls.begin(), walls.end(), rng);

    DSU dsu(width * height);

    auto getIndex = [width](int x, int y) {
        return y * width + x;
    };

    for (const auto& [x1, y1, x2, y2] : walls) {
        int idx1 = getIndex(x1, y1);
        int idx2 = getIndex(x2, y2);

        if (dsu.find(idx1) != dsu.find(idx2)) {
            dsu.unite(idx1, idx2);

            int mx = x1 + x2 + 1;
            int my = y1 + y2 + 1;
            maze[my][mx] = false;
            maze[y1 * 2 + 1][x1 * 2 + 1] = false;
            maze[y2 * 2 + 1][x2 * 2 + 1] = false;
        }
    }

    maze[1][0] = false;
    maze[rows - 2][cols - 1] = false;

    return maze;
}