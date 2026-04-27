#ifndef DIFFICULTYSETTINGS_H
#define DIFFICULTYSETTINGS_H

enum Difficulty { EASY, MEDIUM, HARD };

struct GeneratorSettings {
    int width;
    int height;
    Difficulty difficulty;

    GeneratorSettings() : width(21), height(15), difficulty(MEDIUM) {}

    void setDifficulty(Difficulty d) {
        difficulty = d;
        switch(d) {
        case EASY:
            width = 15;
            height = 11;
            break;
        case MEDIUM:
            width = 21;
            height = 15;
            break;
        case HARD:
            width = 31;
            height = 21;
            break;
        }
    }
};

#endif