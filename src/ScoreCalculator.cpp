
#include "../include/ScoreCalculator.h"

int ScoreCalculator::calculate(
    const std::array<int, 5>& dice,
    Category category
) {
    if (!isValidDice(dice)) {
        return 0;
    }

    const Counts counts = countValues(dice);

    switch (category) {
        case Category::Ones:
        case Category::Twos:
        case Category::Threes:
        case Category::Fours:
        case Category::Fives:
        case Category::Sixes: {
            const int value = static_cast<int>(category) + 1;
            return counts[static_cast<std::size_t>(value)] * value;
        }

        case Category::ThreeOfAKind:
            return hasSameValues(counts, 3) ? sumAll(dice) : 0;

        case Category::FourOfAKind:
            return hasSameValues(counts, 4) ? sumAll(dice) : 0;

        case Category::FullHouse:
            return isFullHouse(counts) ? 25 : 0;

        case Category::SmallStraight:
            return hasStraight(counts, 4) ? 30 : 0;

        case Category::LargeStraight:
            return hasStraight(counts, 5) ? 40 : 0;

        case Category::Yatzy:
            return hasSameValues(counts, 5) ? 50 : 0;

        case Category::Chance:
            return sumAll(dice);
    }

    return 0;
}

bool ScoreCalculator::isValidDice(
    const std::array<int, 5>& dice
) {
    for (std::size_t i = 0; i < dice.size(); ++i) {
        if (dice[i] < 1 || dice[i] > 6) {
            return false;
        }
    }

    return true;
}

ScoreCalculator::Counts ScoreCalculator::countValues(
    const std::array<int, 5>& dice
) {
    Counts counts{};

    for (std::size_t i = 0; i < dice.size(); ++i) {
        ++counts[static_cast<std::size_t>(dice[i])];
    }

    return counts;
}

int ScoreCalculator::sumAll(
    const std::array<int, 5>& dice
) {
    int sum = 0;

    for (std::size_t i = 0; i < dice.size(); ++i) {
        sum += dice[i];
    }

    return sum;
}

bool ScoreCalculator::hasSameValues(
    const Counts& counts,
    int minCount
) {
    for (std::size_t value = 1; value < counts.size(); ++value) {
        if (counts[value] >= minCount) {
            return true;
        }
    }

    return false;
}

bool ScoreCalculator::isFullHouse(const Counts& counts) {
    bool hasThree = false;
    bool hasTwo = false;

    for (std::size_t value = 1; value < counts.size(); ++value) {
        hasThree = hasThree || counts[value] == 3;
        hasTwo = hasTwo || counts[value] == 2;
    }

    return hasThree && hasTwo;
}

bool ScoreCalculator::hasStraight(
    const Counts& counts,
    int length
) {
    int run = 0;

    for (std::size_t value = 1; value < counts.size(); ++value) {
        run = counts[value] > 0 ? run + 1 : 0;

        if (run >= length) {
            return true;
        }
    }

    return false;
}
