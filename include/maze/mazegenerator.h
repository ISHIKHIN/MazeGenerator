#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <vector>
#include <QString>

// ============================================
// ПАТТЕРН "СТРАТЕГИЯ"
// Базовый интерфейс для всех алгоритмов генерации лабиринта
// ============================================
class IMazeGenerator {
public:
    virtual ~IMazeGenerator() = default;

    // Основной метод генерации
    virtual std::vector<std::vector<bool>> generate(int width, int height) = 0;

    // Возвращает имя стратегии (для отображения в UI)
    virtual QString name() const = 0;
};

// ============================================
// КОНКРЕТНАЯ СТРАТЕГИЯ 1: DFS (Поиск в глубину)
// ============================================
class DFSGenerator : public IMazeGenerator {
public:
    std::vector<std::vector<bool>> generate(int width, int height) override;
    QString name() const override { return "DFS (Поиск в глубину)"; }
};

// ============================================
// КОНКРЕТНАЯ СТРАТЕГИЯ 2: Алгоритм Крускала
// ============================================
class KruskalGenerator : public IMazeGenerator {
public:
    std::vector<std::vector<bool>> generate(int width, int height) override;
    QString name() const override { return "Крускал"; }
};

#endif // MAZEGENERATOR_H