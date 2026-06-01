#include "pathfinder.h"
#include <queue>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <limits>

std::vector<std::pair<int, int>> BFSFinder::findPath(
    const std::vector<std::vector<bool>>& maze,
    std::pair<int, int> start,
    std::pair<int, int> end)
{
    if (maze.empty() || maze[0].empty()) return {};

    int rows = (int)maze.size();
    int cols = (int)maze[0].size();

    // Проверка границ
    if (start.first < 0 || start.first >= cols || start.second < 0 || start.second >= rows ||
        end.first < 0 || end.first >= cols || end.second < 0 || end.second >= rows) {
        return {};
    }

    // Если старт или финиш - стена
    if (maze[start.second][start.first] || maze[end.second][end.first]) {
        return {};
    }

    // Если старт = финиш
    if (start == end) {
        return {start};
    }

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));

    std::queue<std::pair<int, int>> q;
    q.push(start);
    visited[start.second][start.first] = true;

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    bool found = false;

    while (!q.empty() && !found) {
        auto [x, y] = q.front();
        q.pop();

        if (std::make_pair(x, y) == end) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < cols && ny < rows &&
                !maze[ny][nx] && !visited[ny][nx])
            {
                visited[ny][nx] = true;
                parent[ny][nx] = {x, y};
                q.push({nx, ny});
            }
        }
    }

    if (!found) {
        return {};
    }

    std::vector<std::pair<int, int>> path;
    auto cur = end;

    while (cur != start) {
        path.push_back(cur);
        cur = parent[cur.second][cur.first];
        if (cur.first == -1 && cur.second == -1) {
            return {};
        }
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<std::pair<int, int>> AStarFinder::findPath(
    const std::vector<std::vector<bool>>& maze,
    std::pair<int, int> start,
    std::pair<int, int> end)
{
    if (maze.empty() || maze[0].empty()) return {};

    int rows = (int)maze.size();
    int cols = (int)maze[0].size();

    if (start.first < 0 || start.first >= cols || start.second < 0 || start.second >= rows ||
        end.first < 0 || end.first >= cols || end.second < 0 || end.second >= rows) {
        return {};
    }

    if (maze[start.second][start.first] || maze[end.second][end.first]) {
        return {};
    }

    if (start == end) {
        return {start};
    }

    auto heuristic = [](int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    };

    using Node = std::tuple<int, int, int, int>; // f, g, x, y
    auto cmp = [](const Node& a, const Node& b) {
        return std::get<0>(a) > std::get<0>(b);
    };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openSet(cmp);

    std::vector<std::vector<int>> gScore(rows, std::vector<int>(cols, std::numeric_limits<int>::max()));
    std::vector<std::vector<int>> fScore(rows, std::vector<int>(cols, std::numeric_limits<int>::max()));
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));

    gScore[start.second][start.first] = 0;
    fScore[start.second][start.first] = heuristic(start.first, start.second, end.first, end.second);
    openSet.push({fScore[start.second][start.first], gScore[start.second][start.first], start.first, start.second});

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    bool found = false;

    while (!openSet.empty() && !found) {
        auto [current_f, current_g, x, y] = openSet.top();
        openSet.pop();

        if (current_f > fScore[y][x]) {
            continue;
        }

        if (x == end.first && y == end.second) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < cols && ny < rows && !maze[ny][nx]) {
                int tentative_g = gScore[y][x] + 1;

                if (tentative_g < gScore[ny][nx]) {
                    parent[ny][nx] = {x, y};
                    gScore[ny][nx] = tentative_g;
                    fScore[ny][nx] = tentative_g + heuristic(nx, ny, end.first, end.second);
                    openSet.push({fScore[ny][nx], tentative_g, nx, ny});
                }
            }
        }
    }

    if (!found) {
        return {};
    }

    std::vector<std::pair<int, int>> path;
    auto cur = end;

    while (cur != start) {
        path.push_back(cur);
        cur = parent[cur.second][cur.first];
        if (cur.first == -1 && cur.second == -1) {
            return {};
        }
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

// Только реализации name() для BFSFinder и AStarFinder
// DFSGenerator::name() и KruskalGenerator::name() уже есть в mazegenerator.cpp
QString BFSFinder::name() const {
    return "BFS (Поиск в ширину)";
}

QString AStarFinder::name() const {
    return "A* (Звездочка)";
}