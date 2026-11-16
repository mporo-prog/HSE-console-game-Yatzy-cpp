
#pragma once

#include <cstddef>
#include "ScoreCard.h"

class Player {
public:
    static constexpr std::size_t MAX_NAME_SIZE = 64;

    /// Создаёт игрока с пустым именем.
    Player();

    /// Создаёт игрока с указанным именем.
    explicit Player(const char* name);

    /// Изменяет имя игрока.
    /// Длинные имена обрезаются через Text::copy.
    void setName(const char* name);

    /// Возвращает имя игрока.
    const char* getName() const;

    /// Возвращает таблицу очков для изменения.
    ScoreCard& getScoreCard();

    /// Возвращает таблицу очков только для чтения.
    const ScoreCard& getScoreCard() const;

private:
    char m_name[MAX_NAME_SIZE] = {};
    ScoreCard m_scoreCard;
};
