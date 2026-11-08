#pragma once

#include <string>
#include "ScoreCard.h"

class Player {
private:
    std::string name;
    ScoreCard scoreCard;

public:
    /// По умолчанию нужно вписать имя игрока
    Player(const std::string& playerName);
    /// Возвращает имя игрока
    const std::string& getName() const;
    /// Возвращает таблицу счета
    ScoreCard& getScoreCard();
};
