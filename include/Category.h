#pragma once

enum class Category {
    Ones,
    Twos,
    Threes,
    Fours,
    Fives,
    Sixes,
    ThreeOfAKind,
    FourOfAKind,
    FullHouse,
    SmallStraight,
    LargeStraight,
    Yatzy,
    Chance
};

/// Общее количество категорий.
constexpr int CATEGORY_COUNT = 13;

/// Количество категорий верхней части таблицы.
constexpr int UPPER_CATEGORY_COUNT = 6;

/// Возвращает название категории для вывода на экран.
const char* getCategoryName(Category category);