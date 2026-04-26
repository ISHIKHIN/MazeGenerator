#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <utility>
#include <QString>

// ============================================
// ПАТТЕРН "СТРАТЕГИЯ"
// Базовый интерфейс для всех алгоритмов поиска пути
// ============================================
class IPathFinder {
public:
    virtual ~IPathFinder() = default;

    // Основной метод поиска пути
    virtual std::vector<std::pair<int, int>> findPath(
        const std::vector<std::vector<bool>>& maze,
        std::pair<int, int> start,
        std::pair<int, int> end) = 0;

    // Возвращает имя стратегии (для отображения в UI)
    virtual QString name() const = 0;
};

// ============================================
// КОНКРЕТНАЯ СТРАТЕГИЯ 1: BFS (Поиск в ширину)
// Гарантирует кратчайший путь в количестве шагов
// ============================================
class BFSFinder : public IPathFinder {
public:
    std::vector<std::pair<int, int>> findPath(
        const std::vector<std::vector<bool>>& maze,
        std::pair<int, int> start,
        std::pair<int, int> end) override;

    QString name() const override { return "BFS (Поиск в ширину)"; }
};

// ============================================
// КОНКРЕТНАЯ СТРАТЕГИЯ 2: A* (Алгоритм Астар)
// Использует эвристику для ускорения поиска
// ============================================
class AStarFinder : public IPathFinder {
public:
    std::vector<std::pair<int, int>> findPath(
        const std::vector<std::vector<bool>>& maze,
        std::pair<int, int> start,
        std::pair<int, int> end) override;

    QString name() const override { return "A* (Звездочка)"; }
};

#endif // PATHFINDER_H