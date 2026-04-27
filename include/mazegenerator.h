#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <vector>
#include <QString>

class IMazeGenerator {
public:
    virtual ~IMazeGenerator() = default;

    virtual std::vector<std::vector<bool>> generate(int width, int height) = 0;

    virtual QString name() const = 0;
};

class DFSGenerator : public IMazeGenerator {
public:
    std::vector<std::vector<bool>> generate(int width, int height) override;
    QString name() const override { return "DFS (Поиск в глубину)"; }
};

class KruskalGenerator : public IMazeGenerator {
public:
    std::vector<std::vector<bool>> generate(int width, int height) override;
    QString name() const override { return "Крускал"; }
};

#endif