#include "../include/DiceSet.h"


DiceSet::DiceSet() {
    for (int i = 0; i < 5; i++) {
        numbers[i] = dis(gen);
        selected[i] = false;
    }
}

void DiceSet::roll() {
    for (int i = 0; i < 5; i++) {
        if (!selected[i]) {
            numbers[i] = dis(gen);
        }
    }
}

void DiceSet::holdDice(int index) {
    if (index >= 0 && index < 5) {
        selected[index] = true;
    }
}

void DiceSet::releaseDice(int index) {
    if (index >= 0 && index < 5) {
        selected[index] = false;
    }
}

const std::array<int, 5>& DiceSet::getDice() const {
    return numbers;
}

void DiceSet::resetDice() {
    for (int i = 0; i < 5; i++) {
        selected[i] = false;
    }
}