#include "../include/ScoreCard.h"

namespace {

    constexpr int UPPER_BONUS_THRESHOLD = 63;
    constexpr int UPPER_BONUS = 35;

}

bool ScoreCard::isFilled(Category category) const {
    return m_filled[toIndex(category)];
}

bool ScoreCard::setScore(Category category, int score) {
    const std::size_t index = toIndex(category);

    if (m_filled[index] || score < 0) {
        return false;
    }

    m_filled[index] = true;
    m_scores[index] = score;

    return true;
}

int ScoreCard::getScore(Category category) const {
    const std::size_t index = toIndex(category);

    return m_filled[index] ? m_scores[index] : -1;
}

int ScoreCard::getUpperSum() const {
    int sum = 0;

    for (std::size_t i = 0; i < UPPER_CATEGORY_COUNT; ++i) {
        sum += m_scores[i];
    }

    return sum;
}

int ScoreCard::getUpperBonus() const {
    return getUpperSum() >= UPPER_BONUS_THRESHOLD
        ? UPPER_BONUS : 0;
}

int ScoreCard::getTotalScore() const {
    int sum = 0;

    for (std::size_t i = 0; i < m_scores.size(); ++i) {
        sum += m_scores[i];
    }

    return sum + getUpperBonus();
}

bool ScoreCard::isComplete() const {
    for (std::size_t i = 0; i < m_filled.size(); ++i) {
        if (!m_filled[i]) {
            return false;
        }
    }

    return true;
}

std::size_t ScoreCard::toIndex(Category category) {
    return static_cast<std::size_t>(category);
}
