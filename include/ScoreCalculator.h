#pragma once
#include <array>
#include "Category.h"

class ScoreCalculator {
public:
    /// При вводе нужно написать кости и категория которая доступа. Возвращаются очки
    static int calculate(const std::array<int, 5>& dice, Category category);

private:
    std::array<int, 7> counts{};
};