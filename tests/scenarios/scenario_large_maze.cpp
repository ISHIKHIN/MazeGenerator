#include <iostream>
#include <iomanip>
#include <chrono>
#include "mazegenerator.h"

#ifdef _WIN32
#include <windows.h>
void setupConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#else
void setupConsole() {}
#endif

int main() {
    setupConsole();

    std::cout << "SCENARIO 4: Large maze generation" << std::endl;

    DFSGenerator dfsGen;
    KruskalGenerator kruskalGen;

    std::vector<std::pair<int, int>> sizes = {
        {20, 20}, {30, 30}, {50, 50}, {70, 70}, {100, 100}
    };

    std::cout << "\n[Performance test of generators]\n" << std::endl;
    std::cout << std::left << std::setw(15) << "Size"
              << std::setw(20) << "DFS time(ms)"
              << std::setw(20) << "Kruskal time(ms)" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    for (const auto& [width, height] : sizes) {
        auto start = std::chrono::high_resolution_clock::now();
        auto dfsMaze = dfsGen.generate(width, height);
        auto end = std::chrono::high_resolution_clock::now();
        auto dfsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        start = std::chrono::high_resolution_clock::now();
        auto kruskalMaze = kruskalGen.generate(width, height);
        end = std::chrono::high_resolution_clock::now();
        auto kruskalTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::string sizeStr = std::to_string(width) + "x" + std::to_string(height);
        std::cout << std::left << std::setw(15) << sizeStr
                  << std::setw(20) << dfsTime.count()
                  << std::setw(20) << kruskalTime.count() << std::endl;
    }

    std::cout << "\n[Maze density analysis]\n" << std::endl;

    std::vector<int> analysisSizes = {10, 20, 30, 40, 50};

    std::cout << std::left << std::setw(15) << "Size"
              << std::setw(20) << "DFS walls(%)"
              << std::setw(20) << "Kruskal walls(%)" << std::endl;
    std::cout << std::string(55, '-') << std::endl;

    for (int size : analysisSizes) {
        auto dfsMaze = dfsGen.generate(size, size);
        auto kruskalMaze = kruskalGen.generate(size, size);

        int dfsWalls = 0, kruskalWalls = 0;
        int totalCells = dfsMaze.size() * dfsMaze[0].size();

        for (const auto& row : dfsMaze) {
            for (bool cell : row) {
                if (cell) dfsWalls++;
            }
        }
        for (const auto& row : kruskalMaze) {
            for (bool cell : row) {
                if (cell) kruskalWalls++;
            }
        }

        double dfsPercent = 100.0 * dfsWalls / totalCells;
        double kruskalPercent = 100.0 * kruskalWalls / totalCells;

        std::cout << std::left << std::setw(15) << (std::to_string(size) + "x" + std::to_string(size))
                  << std::setw(20) << std::fixed << std::setprecision(1) << dfsPercent
                  << std::setw(20) << std::fixed << std::setprecision(1) << kruskalPercent << std::endl;
    }

    std::cout << "Scenario completed successfully!" << std::endl;

    return 0;
}