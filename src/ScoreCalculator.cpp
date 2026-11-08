#include "../include/ScoreCalculator.h"


int ScoreCalculator::calculate(const std::array<int, 5> &dice, Category category) {

    int sum = 0;

    int counts[7] = {};
    bool flag = false;
    for (int i = 0; i < 5; i++) {
        counts[dice[i]]++;
    }

    switch (category) {
        case Category::Chance: {
            for (int i = 0; i < 5; i++) {
                sum += dice[i];
            }
            return sum;
        }

        case Category::Ones: {
            for (int i = 0; i < 5; i++) {
                if (dice[i] == 1) {
                    sum += dice[i];
                }
            }
            return sum;
        }

        case Category::Twos: {
            for (int i = 0; i < 5; i++) {
                if (dice[i] == 2) {
                    sum += dice[i];
                }
            }
            return sum;
        }

        case Category::Threes: {
            for (int i = 0; i < 5; i++) {
                if (dice[i] == 3) {
                    sum += dice[i];
                }
            }
            return sum;
        }

        case Category::Fours: {
            for (int i = 0; i < 5; i++) {
                if (dice[i] == 4) {
                    sum += dice[i];
                }
            }
            return sum;
        }

        case Category::Fives: {
            for (int i = 0; i < 5; i++) {
                if (dice[i] == 5) {
                    sum += dice[i];
                }
            }
            return sum;
        }

        case Category::Sixes: {
            for (int i = 0; i < 5; i++) {
                if (dice[i] == 6) {
                    sum += dice[i];
                }
            }
            return sum;
        }

        case Category::ThreeOfAKind: {
            for (int i = 0; i < 5; i++) {
                sum += dice[i];
            }
            for (int i = 0; i < 7; i++) {
                if (counts[i] >= 3) {
                    flag = true;
                }
            }
            return flag ? sum : 0;
        }

        case Category::FourOfAKind: {
            for (int i = 0; i < 5; i++) {
                sum += dice[i];
            }
            for (int i = 0; i < 7; i++) {
                if (counts[i] >= 4) {
                    flag = true;
                }
            }
            return flag ? sum : 0;
        }

        case Category::Yatzee: {
            for (int i = 0; i < 7; i++) {
                if (counts[i] == 5) {
                    flag = true;
                }
            }
            return flag ? 50 : 0;
        }

        case Category::FullHouse: {
            bool flagThree = false;
            bool flagTwo = false;

            for (int i = 0; i < 7; i++) {
                if (counts[i] == 3) {
                    flagThree = true;
                }
                if (counts[i] == 2) {
                    flagTwo = true;
                }
            }
            return (flagThree && flagTwo) ? 25 : 0;
        }

        case Category::SmallStraight: {
            if ((counts[1] > 0 && counts[2] > 0 && counts[3] > 0 && counts[4] > 0) ||
                (counts[2] > 0 && counts[3] > 0 && counts[4] > 0 && counts[5] > 0) ||
                (counts[3] > 0 && counts[4] > 0 && counts[5] > 0 && counts[6] > 0)) {
                flag = true;
            }
            return flag ? 30 : 0;
        }

        case Category::LargeStraight: {
            if ((counts[1] > 0 && counts[2] > 0 && counts[3] > 0 && counts[4] > 0 && counts[5] > 0) ||
                (counts[2] > 0 && counts[3] > 0 && counts[4] > 0 && counts[5] > 0 && counts[6] > 0)) {
                flag = true;
            }
            return flag ? 40 : 0;
        }
    }
    return 0;
}
