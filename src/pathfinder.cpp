#include "maze/pathfinder.h"
#include <queue>
#include <algorithm>
#include <cmath>
#include <unordered_map>

// ============================================
// ВСПОМОГАТЕЛЬНАЯ СТРУКТУРА ДЛЯ A*
// Узел с координатами, стоимостью и указателем на родителя
// ============================================
struct AStarNode {
    int x, y;           // Координаты
    int g;              // Стоимость пути от старта
    int h;              // Эвристика (расстояние до цели)
    AStarNode* parent;  // Предыдущий узел в пути

    AStarNode(int x_, int y_) : x(x_), y(y_), g(0), h(0), parent(nullptr) {}

    int f() const { return g + h; }  // Общая стоимость

    bool operator>(const AStarNode& other) const {
        return f() > other.f();
    }
};

// Эвристическая функция для A* (манхэттенское расстояние)
static int heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// ============================================
// РЕАЛИЗАЦИЯ СТРАТЕГИИ BFS
// ============================================
std::vector<std::pair<int, int>> BFSFinder::findPath(
    const std::vector<std::vector<bool>>& maze,
    std::pair<int, int> start,
    std::pair<int, int> end)
{
    int rows = (int)maze.size();
    int cols = (int)maze[0].size();

    // Матрицы посещённых клеток и родителей
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));

    std::queue<std::pair<int, int>> q;
    q.push(start);
    visited[start.second][start.first] = true;

    // Направления движения (вверх, вниз, влево, вправо)
    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // Достигли финиша?
        if (std::make_pair(x, y) == end) {
            break;
        }

        // Проверка соседей
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

    // Восстановление пути от финиша к старту
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

// ============================================
// РЕАЛИЗАЦИЯ СТРАТЕГИИ A*
// ============================================
std::vector<std::pair<int, int>> AStarFinder::findPath(
    const std::vector<std::vector<bool>>& maze,
    std::pair<int, int> start,
    std::pair<int, int> end)
{
    int rows = (int)maze.size();
    int cols = (int)maze[0].size();

    // Очередь с приоритетом (минимальная стоимость f)
    auto cmp = [](AStarNode* a, AStarNode* b) {
        return a->f() > b->f();
    };
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, decltype(cmp)> openSet(cmp);

    // Закрытые вершины (уже обработанные)
    std::vector<std::vector<bool>> closedSet(rows, std::vector<bool>(cols, false));

    // Хранилище всех узлов для последующего удаления
    std::vector<std::vector<AStarNode*>> nodes(rows, std::vector<AStarNode*>(cols, nullptr));

    // Стартовый узел
    AStarNode* startNode = new AStarNode(start.first, start.second);
    startNode->g = 0;
    startNode->h = heuristic(start.first, start.second, end.first, end.second);
    startNode->parent = nullptr;

    openSet.push(startNode);
    nodes[start.second][start.first] = startNode;

    // Направления движения
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

        // Достигли цели?
        if (current->x == end.first && current->y == end.second) {
            endNode = current;
            break;
        }

        // Проверка соседей
        for (int i = 0; i < 4; ++i) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < cols && ny < rows &&
                !maze[ny][nx] && !closedSet[ny][nx])
            {
                int tentative_g = current->g + 1;

                // Создаём узел, если его ещё нет
                if (nodes[ny][nx] == nullptr) {
                    nodes[ny][nx] = new AStarNode(nx, ny);
                }

                AStarNode* neighbor = nodes[ny][nx];

                // Если нашли более короткий путь к соседу
                if (tentative_g < neighbor->g) {
                    neighbor->parent = current;
                    neighbor->g = tentative_g;
                    neighbor->h = heuristic(nx, ny, end.first, end.second);
                    openSet.push(neighbor);
                }
            }
        }
    }

    // Восстановление пути
    std::vector<std::pair<int, int>> path;
    for (AStarNode* cur = endNode; cur != nullptr; cur = cur->parent) {
        path.push_back({cur->x, cur->y});
    }
    std::reverse(path.begin(), path.end());

    // Очистка памяти
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            delete nodes[y][x];
        }
    }

    return path;
}