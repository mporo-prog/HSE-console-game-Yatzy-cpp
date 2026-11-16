#pragma once

#include <array>
#include "Category.h"

class ScoreCalculator {
public:
    static int calculate(
        const std::array<int, 5>& dice,
        Category category
    );

private:
    /// Количество выпадений каждого значения кубика.
    using Counts = std::array<int, 7>;

    /// Проверяет значения кубиков от 1 до 6.
    static bool isValidDice(const std::array<int, 5>& dice);

    /// Считает количество выпадений каждого значения.
    static Counts countValues(const std::array<int, 5>& dice);

    /// Возвращает сумму всех кубиков.
    static int sumAll(const std::array<int, 5>& dice);

    /// Проверяет наличие minCount одинаковых кубиков.
    static bool hasSameValues(
        const Counts& counts,
        int minCount
    );

    /// Проверяет комбинацию из трёх одинаковых и пары.
    static bool isFullHouse(const Counts& counts);

    /// Проверяет последовательность заданной длины.
    static bool hasStraight(
        const Counts& counts,
        int length
    );
};
