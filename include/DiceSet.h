
#pragma once

#include <array>
#include <random>

class DiceSet {
public:
    /// Количество кубиков.
    static constexpr int DICE_COUNT = 5;

    /// Создаёт набор и сразу бросает все кубики.
    DiceSet();

    /// Перебрасывает все незафиксированные кубики.
    void roll();

    /// Фиксирует кубик с индексом от 0 до 4.
    bool holdDice(int index);

    /// Снимает фиксацию с кубика.
    bool releaseDice(int index);

    /// Проверяет, зафиксирован ли кубик.
    bool isHeld(int index) const;

    /// Снимает фиксацию со всех кубиков.
    void resetDice();

    /// Возвращает значения всех пяти кубиков.
    const std::array<int, DICE_COUNT>& getDice() const;

private:
    /// Проверяет, что индекс находится в диапазоне 0-4.
    static bool isValidIndex(int index);

    std::mt19937 m_generator;
    std::uniform_int_distribution<int> m_distribution{1, 6};

    std::array<int, DICE_COUNT> m_values{};
    std::array<bool, DICE_COUNT> m_held{};
};