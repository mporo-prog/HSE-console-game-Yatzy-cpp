
#include "../include/DiceSet.h"

#include <chrono>

namespace {

    /// Создаёт начальное значение для генератора случайных чисел.
    std::mt19937::result_type makeSeed() {
        const auto timeSeed =
            static_cast<std::mt19937::result_type>(
                std::chrono::steady_clock::now()
                    .time_since_epoch().count()
            );

        return std::random_device{}() ^ timeSeed;
    }

}

DiceSet::DiceSet() : m_generator(makeSeed()) {
    roll();
}

void DiceSet::roll() {
    for (std::size_t i = 0; i < m_values.size(); ++i) {
        if (!m_held[i]) {
            m_values[i] = m_distribution(m_generator);
        }
    }
}

bool DiceSet::holdDice(int index) {
    if (!isValidIndex(index)) {
        return false;
    }

    m_held[static_cast<std::size_t>(index)] = true;
    return true;
}

bool DiceSet::releaseDice(int index) {
    if (!isValidIndex(index)) {
        return false;
    }

    m_held[static_cast<std::size_t>(index)] = false;
    return true;
}

bool DiceSet::isHeld(int index) const {
    return isValidIndex(index) &&
           m_held[static_cast<std::size_t>(index)];
}

void DiceSet::resetDice() {
    m_held.fill(false);
}

const std::array<int, DiceSet::DICE_COUNT>&
DiceSet::getDice() const {
    return m_values;
}

bool DiceSet::isValidIndex(int index) {
    return index >= 0 && index < DICE_COUNT;
}
