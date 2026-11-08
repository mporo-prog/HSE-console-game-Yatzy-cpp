#pragma once

#include <array>
#include "Category.h"

class ScoreCard {
private:
    std::array<int, 13> scores{};
    std::array<int, 13> filled{};

public:
    /// Проверка если уже стоит число в таблице
    bool isFilled(Category category) const;
    /// Устанавливает число в конкретную ячейку в таблице
    bool setScore(Category category, int score);
    /// Возвращает число в конкретной ячейке, если там пусто возвращает -1
    int getScore(Category category) const;
    /// Если в верхней таблице игрок собрал 63, то он получает бонус
    int getUpperBonus() const;
    /// Возвращает сумму всех очков игрока
    int getTotalScore() const;
};
