#pragma once

#include <random>
#include <array>

class DiceSet {
private:
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> dis{1, 6};

    std::array<int, 5> numbers{};
    std::array<bool, 5> selected{};

public:
    /// Когда в int main пишешь DiceSet dice, то уже по умолчанию кидается кубик
    DiceSet();
    /// Кидает заново кубики (кубики которые мы сохраняем остаются)
    void roll();
    /// Сохраняем 1 кубик (ИМЕННО КУБИК, а НЕ ЗНАЧЕНИЕ)
    void holdDice(int index);
    /// Перестаем сохранять кубик
    void releaseDice(int index);
    /// Возвращает все кубики
    const std::array<int, 5>& getDice() const;
    /// Убирает все "замки" на кубиках
    void resetDice();
};