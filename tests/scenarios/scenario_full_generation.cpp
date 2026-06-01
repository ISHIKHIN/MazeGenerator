#include <iostream>
#include <chrono>
#include "mazegenerator.h"
#include "pathfinder.h"

#ifdef _WIN32
#include <windows.h>
void setupConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#else
void setupConsole() {}
#endif

void printMaze(const std::vector<std::vector<bool>>& maze) {
    for (const auto& row : maze) {
        for (bool cell : row) {
            std::cout << (cell ? "##" : "  ");
        }
        std::cout << std::endl;
    }
}

int main() {
    setupConsole();

    std::cout << "SCENARIO 1: Full generation and path finding" << std::endl;
    // Step 1: Configure parameters
    int width = 15;
    int height = 11;
    std::cout << "\n[Step 1] Maze parameters: " << width << "x" << height << std::endl;

    // Step 2: Generate maze using DFS
    std::cout << "[Step 2] Generating maze (DFS)..." << std::endl;
    DFSGenerator dfsGen;
    auto startGen = std::chrono::high_resolution_clock::now();
    auto maze = dfsGen.generate(width, height);
    auto endGen = std::chrono::high_resolution_clock::now();
    auto genTime = std::chrono::duration_cast<std::chrono::milliseconds>(endGen - startGen);

    std::cout << "  - Generation time: " << genTime.count() << " ms" << std::endl;
    std::cout << "  - Maze size: " << maze.size() << "x" << maze[0].size() << std::endl;

    // Step 4: Find path using BFS
    std::cout << "\n[Step 3] Finding path (BFS)..." << std::endl;
    BFSFinder bfsFinder;
    std::pair<int, int> start = {1, 1};
    std::pair<int, int> end = {(int)maze[0].size() - 2, (int)maze.size() - 2};
    std::cout << "  - Start: (" << start.first << "," << start.second << ")" << std::endl;
    std::cout << "  - Finish: (" << end.first << "," << end.second << ")" << std::endl;

    auto startFind = std::chrono::high_resolution_clock::now();
    auto path = bfsFinder.findPath(maze, start, end);
    auto endFind = std::chrono::high_resolution_clock::now();
    auto findTime = std::chrono::duration_cast<std::chrono::microseconds>(endFind - startFind);

    std::cout << "  - Search time: " << findTime.count() << " us" << std::endl;
    std::cout << "  - Path length: " << path.size() << " steps" << std::endl;

    // Step 5: Check result
    std::cout << "\n[Step 4] Result check:" << std::endl;
    if (!path.empty()) {
        std::cout << "  [OK] Path successfully found!" << std::endl;
        std::cout << "  [OK] Start point: (" << path.front().first << "," << path.front().second << ")" << std::endl;
        std::cout << "  [OK] End point: (" << path.back().first << "," << path.back().second << ")" << std::endl;

        // Check path validity
        bool valid = true;
        for (size_t i = 1; i < path.size(); ++i) {
            int dx = std::abs(path[i].first - path[i-1].first);
            int dy = std::abs(path[i].second - path[i-1].second);
            if (dx + dy != 1) {
                valid = false;
                break;
            }
        }
        std::cout << "  [OK] Path valid: " << (valid ? "Yes" : "No") << std::endl;
    } else {
        std::cout << "  [FAIL] Path not found!" << std::endl;
    }

    // Step 6: Generate maze using Kruskal
    std::cout << "\n[Step 5] Generating maze using Kruskal..." << std::endl;
    KruskalGenerator kruskalGen;
    startGen = std::chrono::high_resolution_clock::now();
    auto maze2 = kruskalGen.generate(width, height);
    endGen = std::chrono::high_resolution_clock::now();
    genTime = std::chrono::duration_cast<std::chrono::milliseconds>(endGen - startGen);
    std::cout << "  - Generation time: " << genTime.count() << " ms" << std::endl;

    // Step 7: Find path in Kruskal maze using A*
    std::cout << "\n[Step 6] Finding path (A*) in Kruskal maze..." << std::endl;
    AStarFinder astarFinder;
    startFind = std::chrono::high_resolution_clock::now();
    auto path2 = astarFinder.findPath(maze2, start, end);
    endFind = std::chrono::high_resolution_clock::now();
    findTime = std::chrono::duration_cast<std::chrono::microseconds>(endFind - startFind);

    std::cout << "  - Search time: " << findTime.count() << " us" << std::endl;
    std::cout << "  - Path length: " << path2.size() << " steps" << std::endl;
    std::cout << "  - Path found: " << (!path2.empty() ? "Yes" : "No") << std::endl;

    std::cout << "Scenario completed successfully!" << std::endl;

    return 0;
}