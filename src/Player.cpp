
#include "../include/Player.h"
#include "../include/Text.h"

Player::Player() = default;

Player::Player(const char* name) {
    setName(name);
}

void Player::setName(const char* name) {
    Text::copy(m_name, MAX_NAME_SIZE, name);
}

const char* Player::getName() const {
    return m_name;
}

ScoreCard& Player::getScoreCard() {
    return m_scoreCard;
}

const ScoreCard& Player::getScoreCard() const {
    return m_scoreCard;
}
