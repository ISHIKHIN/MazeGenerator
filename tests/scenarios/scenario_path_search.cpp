#include <iostream>
#include <iomanip>
#include <chrono>
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

int main() {
    setupConsole();

    std::cout << "SCENARIO 2: Path finding in various conditions" << std::endl;

    BFSFinder bfsFinder;
    AStarFinder astarFinder;

    // Create empty maze
    std::vector<std::vector<bool>> emptyMaze10(10, std::vector<bool>(10, false));
    std::vector<std::vector<bool>> emptyMaze20(20, std::vector<bool>(20, false));
    std::vector<std::vector<bool>> emptyMaze50(50, std::vector<bool>(50, false));

    std::pair<int, int> start = {0, 0};
    std::pair<int, int> end10 = {9, 9};
    std::pair<int, int> end20 = {19, 19};
    std::pair<int, int> end50 = {49, 49};

    std::cout << "\nTest 1: Empty maze 10x10" << std::endl;
    auto bfsPath = bfsFinder.findPath(emptyMaze10, start, end10);
    auto astarPath = astarFinder.findPath(emptyMaze10, start, end10);
    std::cout << "  BFS: path found = " << (!bfsPath.empty() ? "Yes" : "No") << ", length = " << bfsPath.size() << std::endl;
    std::cout << "  A*:  path found = " << (!astarPath.empty() ? "Yes" : "No") << ", length = " << astarPath.size() << std::endl;

    std::cout << "\nTest 2: Empty maze 20x20" << std::endl;
    bfsPath = bfsFinder.findPath(emptyMaze20, start, end20);
    astarPath = astarFinder.findPath(emptyMaze20, start, end20);
    std::cout << "  BFS: path found = " << (!bfsPath.empty() ? "Yes" : "No") << ", length = " << bfsPath.size() << std::endl;
    std::cout << "  A*:  path found = " << (!astarPath.empty() ? "Yes" : "No") << ", length = " << astarPath.size() << std::endl;

    std::cout << "\nTest 3: Impassable maze" << std::endl;
    std::vector<std::vector<bool>> blockedMaze(10, std::vector<bool>(10, false));
    for (int i = 0; i < 10; ++i) {
        blockedMaze[i][5] = true;
    }
    bfsPath = bfsFinder.findPath(blockedMaze, start, end10);
    astarPath = astarFinder.findPath(blockedMaze, start, end10);
    std::cout << "  BFS: path found = " << (!bfsPath.empty() ? "Yes" : "No") << std::endl;
    std::cout << "  A*:  path found = " << (!astarPath.empty() ? "Yes" : "No") << std::endl;

    std::cout << "\nTest 4: Large maze 50x50" << std::endl;
    bfsPath = bfsFinder.findPath(emptyMaze50, start, end50);
    astarPath = astarFinder.findPath(emptyMaze50, start, end50);
    std::cout << "  BFS: path found = " << (!bfsPath.empty() ? "Yes" : "No") << ", length = " << bfsPath.size() << std::endl;
    std::cout << "  A*:  path found = " << (!astarPath.empty() ? "Yes" : "No") << ", length = " << astarPath.size() << std::endl;

    std::cout << "Scenario completed successfully!" << std::endl;

    return 0;
}