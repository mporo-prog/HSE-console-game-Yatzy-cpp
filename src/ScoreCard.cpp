#include "../include/ScoreCard.h"


bool ScoreCard::isFilled(Category category) const {
    int index = static_cast<int>(category);

    return filled[index];
}

bool ScoreCard::setScore(Category category, int score) {
    int index = static_cast<int>(category);

    if (filled[index] || score < 0) {
        return false;
    }
    filled[index] = true;
    scores[index] = score;
    return true;
}

int ScoreCard::getScore(Category category) const {
    int index = static_cast<int>(category);

    if (!filled[index]) {
        return -1;
    }
    return scores[index];
}

int ScoreCard::getUpperBonus() const {
    int sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += scores[i];
    }
    if (sum >= 63) {
        return 35;
    }
    return 0;
}


int ScoreCard::getTotalScore() const {
    int sum = 0;
    for (int i = 0; i < 13; i++) {
        sum += scores[i];
    }

    sum += getUpperBonus();

    return sum;
}