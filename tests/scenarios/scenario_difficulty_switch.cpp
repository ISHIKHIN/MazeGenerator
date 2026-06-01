#include <iostream>
#include <iomanip>
#include <chrono>
#include "mazegenerator.h"
#include "difficultysettings.h"

// Функция для корректного вывода UTF-8 в консоль Windows
#ifdef _WIN32
#include <windows.h>
void setupConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#else
void setupConsole() {}
#endif

class DifficultySwitchScenario {
public:
    void run() {
        setupConsole();

        std::cout << "========================================" << std::endl;
        std::cout << "SCENARIO 3: Difficulty switching" << std::endl;
        std::cout << "========================================" << std::endl;

        DFSGenerator generator;

        std::vector<std::pair<Difficulty, std::string>> difficulties = {
            {EASY, "Easy"},
            {MEDIUM, "Medium"},
            {HARD, "Hard"}
        };

        std::cout << "\n[Step 1] Testing preset difficulty levels:\n" << std::endl;
        std::cout << std::left << std::setw(15) << "Difficulty"
                  << std::setw(15) << "Width"
                  << std::setw(15) << "Height"
                  << std::setw(15) << "Cells"
                  << std::setw(15) << "Time(ms)"
                  << "Maze Size" << std::endl;
        std::cout << std::string(85, '-') << std::endl;

        for (const auto& [diff, name] : difficulties) {
            GeneratorSettings settings;
            settings.setDifficulty(diff);

            auto start = std::chrono::high_resolution_clock::now();
            auto maze = generator.generate(settings.width, settings.height);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            int totalCells = settings.width * settings.height;
            int mazeRows = maze.size();
            int mazeCols = maze[0].size();

            std::cout << std::left << std::setw(15) << name
                      << std::setw(15) << settings.width
                      << std::setw(15) << settings.height
                      << std::setw(15) << totalCells
                      << std::setw(15) << duration.count()
                      << mazeRows << "x" << mazeCols << std::endl;
        }

        std::cout << "\n[Step 2] Dynamic difficulty change:\n" << std::endl;

        GeneratorSettings settings;

        // Start with easy difficulty
        std::cout << "Setting EASY difficulty..." << std::endl;
        settings.setDifficulty(EASY);
        std::cout << "  - Size: " << settings.width << "x" << settings.height << std::endl;
        auto maze1 = generator.generate(settings.width, settings.height);
        std::cout << "  - Maze generated: " << maze1.size() << "x" << maze1[0].size() << std::endl;

        // Change to medium
        std::cout << "\nSetting MEDIUM difficulty..." << std::endl;
        settings.setDifficulty(MEDIUM);
        std::cout << "  - Size: " << settings.width << "x" << settings.height << std::endl;
        auto maze2 = generator.generate(settings.width, settings.height);
        std::cout << "  - Maze generated: " << maze2.size() << "x" << maze2[0].size() << std::endl;

        // Change to hard
        std::cout << "\nSetting HARD difficulty..." << std::endl;
        settings.setDifficulty(HARD);
        std::cout << "  - Size: " << settings.width << "x" << settings.height << std::endl;
        auto maze3 = generator.generate(settings.width, settings.height);
        std::cout << "  - Maze generated: " << maze3.size() << "x" << maze3[0].size() << std::endl;

        std::cout << "\n[Step 3] Checking difficulty switch correctness:\n" << std::endl;

        bool allCorrect = true;

        // Check that dimensions match the selected difficulty
        settings.setDifficulty(EASY);
        if (settings.width == 15 && settings.height == 11) {
            std::cout << "  [OK] Easy difficulty: 15x11" << std::endl;
        } else {
            std::cout << "  [FAIL] Easy difficulty: expected 15x11, got "
                      << settings.width << "x" << settings.height << std::endl;
            allCorrect = false;
        }

        settings.setDifficulty(MEDIUM);
        if (settings.width == 21 && settings.height == 15) {
            std::cout << "  [OK] Medium difficulty: 21x15" << std::endl;
        } else {
            std::cout << "  [FAIL] Medium difficulty: expected 21x15, got "
                      << settings.width << "x" << settings.height << std::endl;
            allCorrect = false;
        }

        settings.setDifficulty(HARD);
        if (settings.width == 31 && settings.height == 21) {
            std::cout << "  [OK] Hard difficulty: 31x21" << std::endl;
        } else {
            std::cout << "  [FAIL] Hard difficulty: expected 31x21, got "
                      << settings.width << "x" << settings.height << std::endl;
            allCorrect = false;
        }

        std::cout << "\n[Step 4] Custom sizes:\n" << std::endl;

        // Testing custom sizes
        std::vector<std::pair<int, int>> customSizes = {
            {10, 10}, {25, 20}, {40, 30}, {50, 50}, {100, 80}
        };

        std::cout << std::left << std::setw(15) << "Width"
                  << std::setw(15) << "Height"
                  << std::setw(15) << "Time(ms)"
                  << "Success" << std::endl;
        std::cout << std::string(50, '-') << std::endl;

        for (const auto& [w, h] : customSizes) {
            auto start = std::chrono::high_resolution_clock::now();
            auto maze = generator.generate(w, h);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            bool success = !maze.empty() && maze.size() == (size_t)(h * 2 + 1);

            std::cout << std::left << std::setw(15) << w
                      << std::setw(15) << h
                      << std::setw(15) << duration.count()
                      << (success ? "[OK]" : "[FAIL]") << std::endl;
        }

        std::cout << "\n========================================" << std::endl;
        if (allCorrect) {
            std::cout << "Scenario completed successfully!" << std::endl;
        } else {
            std::cout << "Scenario completed with errors!" << std::endl;
        }
        std::cout << "========================================" << std::endl;
    }
};

int main() {
    DifficultySwitchScenario scenario;
    scenario.run();
    return 0;
}