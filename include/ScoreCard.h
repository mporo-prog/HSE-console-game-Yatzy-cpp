#pragma once

#include <array>
#include <cstddef>
#include "Category.h"

class ScoreCard {
public:
    /// Возвращает true, если категория уже заполнена.
    bool isFilled(Category category) const;

    /// Записывает очки в свободную категорию.
    bool setScore(Category category, int score);

    /// Возвращает очки категории или -1, если она пустая.
    int getScore(Category category) const;

    /// Возвращает сумму первых шести категорий без бонуса.
    int getUpperSum() const;

    /// Возвращает 35 при сумме верхней части от 63,
    int getUpperBonus() const;

    /// Возвращает общую сумму очков с бонусом.
    int getTotalScore() const;

    /// Возвращает true, если все 13 категорий заполнены.
    bool isComplete() const;

private:
    /// Преобразует категорию в индекс массива.
    static std::size_t toIndex(Category category);

    std::array<int, CATEGORY_COUNT> m_scores{};
    std::array<bool, CATEGORY_COUNT> m_filled{};
};
