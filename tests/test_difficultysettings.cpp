#include <gtest/gtest.h>
#include "difficultysettings.h"

class DifficultySettingsTest : public ::testing::Test {
protected:
    void SetUp() override {
        settings = GeneratorSettings();
    }

    GeneratorSettings settings;
};

// Тест 1: Проверка значений по умолчанию
TEST_F(DifficultySettingsTest, DefaultValues) {
    EXPECT_EQ(settings.width, 21);
    EXPECT_EQ(settings.height, 15);
    EXPECT_EQ(settings.difficulty, MEDIUM);
}

// Тест 2: Установка легкой сложности
TEST_F(DifficultySettingsTest, SetEasyDifficulty) {
    settings.setDifficulty(EASY);
    EXPECT_EQ(settings.width, 15);
    EXPECT_EQ(settings.height, 11);
    EXPECT_EQ(settings.difficulty, EASY);
}

// Тест 3: Установка средней сложности
TEST_F(DifficultySettingsTest, SetMediumDifficulty) {
    settings.setDifficulty(MEDIUM);
    EXPECT_EQ(settings.width, 21);
    EXPECT_EQ(settings.height, 15);
    EXPECT_EQ(settings.difficulty, MEDIUM);
}

// Тест 4: Установка сложной сложности
TEST_F(DifficultySettingsTest, SetHardDifficulty) {
    settings.setDifficulty(HARD);
    EXPECT_EQ(settings.width, 31);
    EXPECT_EQ(settings.height, 21);
    EXPECT_EQ(settings.difficulty, HARD);
}

// Тест 5: Последовательная смена сложности
TEST_F(DifficultySettingsTest, SequentialDifficultyChange) {
    settings.setDifficulty(EASY);
    EXPECT_EQ(settings.width, 15);
    EXPECT_EQ(settings.height, 11);

    settings.setDifficulty(HARD);
    EXPECT_EQ(settings.width, 31);
    EXPECT_EQ(settings.height, 21);

    settings.setDifficulty(MEDIUM);
    EXPECT_EQ(settings.width, 21);
    EXPECT_EQ(settings.height, 15);
}

// Тест 6: Проверка корректности размеров после установки
TEST_F(DifficultySettingsTest, ValidDimensionsAfterSet) {
    settings.setDifficulty(EASY);
    EXPECT_GT(settings.width, 0);
    EXPECT_GT(settings.height, 0);

    settings.setDifficulty(MEDIUM);
    EXPECT_GT(settings.width, 0);
    EXPECT_GT(settings.height, 0);

    settings.setDifficulty(HARD);
    EXPECT_GT(settings.width, 0);
    EXPECT_GT(settings.height, 0);
}

// Тест 7: Прямое изменение размеров
TEST_F(DifficultySettingsTest, DirectDimensionChange) {
    settings.width = 50;
    settings.height = 40;
    EXPECT_EQ(settings.width, 50);
    EXPECT_EQ(settings.height, 40);
}