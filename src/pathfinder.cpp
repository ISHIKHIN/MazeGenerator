#include "maze/pathfinder.h"
#include <queue>
#include <algorithm>
#include <cmath>
#include <unordered_map>

struct AStarNode {
    int x, y;
    int g;
    int h;
    AStarNode* parent;

    AStarNode(int x_, int y_) : x(x_), y(y_), g(0), h(0), parent(nullptr) {}

    int f() const { return g + h; }

    bool operator>(const AStarNode& other) const {
        return f() > other.f();
    }
};

static int heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<std::pair<int, int>> BFSFinder::findPath(
    const std::vector<std::vector<bool>>& maze,
    std::pair<int, int> start,
    std::pair<int, int> end)
{
    int rows = (int)maze.size();
    int cols = (int)maze[0].size();

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));

    std::queue<std::pair<int, int>> q;
    q.push(start);
    visited[start.second][start.first] = true;

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        if (std::make_pair(x, y) == end) {
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

    std::vector<std::pair<int, int>> path;
    auto cur = end;

    while (cur != start && cur != std::make_pair(-1, -1)) {
        path.push_back(cur);
        cur = parent[cur.second][cur.first];
    }

    if (cur == start) {
        path.push_back(start);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<std::pair<int, int>> AStarFinder::findPath(
    const std::vector<std::vector<bool>>& maze,
    std::pair<int, int> start,
    std::pair<int, int> end)
{
    int rows = (int)maze.size();
    int cols = (int)maze[0].size();

    auto cmp = [](AStarNode* a, AStarNode* b) {
        return a->f() > b->f();
    };
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, decltype(cmp)> openSet(cmp);

    std::vector<std::vector<bool>> closedSet(rows, std::vector<bool>(cols, false));

    std::vector<std::vector<AStarNode*>> nodes(rows, std::vector<AStarNode*>(cols, nullptr));

    AStarNode* startNode = new AStarNode(start.first, start.second);
    startNode->g = 0;
    startNode->h = heuristic(start.first, start.second, end.first, end.second);
    startNode->parent = nullptr;

    openSet.push(startNode);
    nodes[start.second][start.first] = startNode;

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    AStarNode* endNode = nullptr;

    while (!openSet.empty()) {
        AStarNode* current = openSet.top();
        openSet.pop();

        if (closedSet[current->y][current->x]) {
            continue;
        }
        closedSet[current->y][current->x] = true;

        if (current->x == end.first && current->y == end.second) {
            endNode = current;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < cols && ny < rows &&
                !maze[ny][nx] && !closedSet[ny][nx])
            {
                int tentative_g = current->g + 1;

                if (nodes[ny][nx] == nullptr) {
                    nodes[ny][nx] = new AStarNode(nx, ny);
                }

                AStarNode* neighbor = nodes[ny][nx];

                if (tentative_g < neighbor->g) {
                    neighbor->parent = current;
                    neighbor->g = tentative_g;
                    neighbor->h = heuristic(nx, ny, end.first, end.second);
                    openSet.push(neighbor);
                }
            }
        }
    }

    std::vector<std::pair<int, int>> path;
    for (AStarNode* cur = endNode; cur != nullptr; cur = cur->parent) {
        path.push_back({cur->x, cur->y});
    }
    std::reverse(path.begin(), path.end());

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            delete nodes[y][x];
        }
    }

    return path;
}