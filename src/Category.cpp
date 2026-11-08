#include "../include/Category.h"

const char* getCategoryName(Category category) {
    switch (category) {
        case Category::Ones:
            return "Ones";

        case Category::Twos:
            return "Twos";

        case Category::Threes:
            return "Threes";

        case Category::Fours:
            return "Fours";

        case Category::Fives:
            return "Fives";

        case Category::Sixes:
            return "Sixes";

        case Category::ThreeOfAKind:
            return "ThreeOfAKind";

        case Category::FourOfAKind:
            return "FourOfAKind";

        case Category::FullHouse:
            return "FullHouse";

        case Category::SmallStraight:
            return "SmallStraight";

        case Category::LargeStraight:
            return "LargeStraight";

        case Category::Yatzee:
            return "Yahtzee";

        case Category::Chance:
            return "Chance";
    }
    return "Unknown";
}