#include "../include/Player.h"


Player::Player(const std::string& playerName) {
    name = playerName;
}

const std::string& Player::getName() const {
    return name;
}

ScoreCard& Player::getScoreCard() {
    return scoreCard;
}