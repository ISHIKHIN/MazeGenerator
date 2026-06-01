#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <utility>
#include <QString>

class IPathFinder {
public:
    virtual ~IPathFinder() = default;

    virtual std::vector<std::pair<int, int>> findPath(
        const std::vector<std::vector<bool>>& maze,
        std::pair<int, int> start,
        std::pair<int, int> end) = 0;

    virtual QString name() const = 0;
};

class BFSFinder : public IPathFinder {
public:
    std::vector<std::pair<int, int>> findPath(
        const std::vector<std::vector<bool>>& maze,
        std::pair<int, int> start,
        std::pair<int, int> end) override;

    QString name() const override;  // только объявление, без реализации
};

class AStarFinder : public IPathFinder {
public:
    std::vector<std::pair<int, int>> findPath(
        const std::vector<std::vector<bool>>& maze,
        std::pair<int, int> start,
        std::pair<int, int> end) override;

    QString name() const override;  // только объявление, без реализации
};

#endif