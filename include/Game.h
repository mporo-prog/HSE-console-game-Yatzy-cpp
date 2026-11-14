#ifndef GAME_H
#define GAME_H
#include <cstddef>
#include "Category.h"
#include "DiceSet.h"
#include "Player.h"
/// Игровой процесс: игроки, раунды, ход из трёх бросков, запись очков.
/// Ввод и вывод не делает — это InputReader и ConsoleRenderer.
class Game {
public:
    /// Размеры и лимиты партии. Открыты, чтобы интерфейс брал отсюда размеры буферов.
    static constexpr std::size_t DICE_COUNT = 5;
    static constexpr std::size_t MIN_PLAYERS = 2;
    static constexpr std::size_t MAX_PLAYERS = 6;
    static constexpr std::size_t MAX_NAME_LENGTH = 32;
    static constexpr std::size_t NAME_BUFFER_SIZE = 128;
    static constexpr int ROUND_COUNT = 13;
    static constexpr int MAX_ROLLS = 3;
    /// Создаёт пустую партию: игроков нет, раунд 0, бросков 0.
    Game();
    /// Добавляет игрока до начала партии.
    /// name — имя в UTF-8: не nullptr, непустое, не длиннее MAX_NAME_LENGTH, не повторяется.
    /// Возвращает false при неверном имени, если партия уже идёт или игроков MAX_PLAYERS.
    bool addPlayer(const char* name);
    /// Начинает партию с добавленными игроками: раунд 1, ходит первый, бросков 0.
    /// Возвращает false, если игроков меньше MIN_PLAYERS.
    bool startNewGame();
    /// Сбрасывает партию и список игроков к состоянию после конструктора.
    void reset();
    /// Бросает неотложенные кубики и увеличивает счётчик бросков.
    /// Возвращает false, если партии нет, она завершена или прервана, либо бросков уже MAX_ROLLS.
    bool rollDice();
    /// Откладывает кубик или возвращает его в игру.
    /// dieIndex — номер кубика 0…DICE_COUNT-1, held — отложить или вернуть.
    /// Возвращает false при неверном индексе или если броска ещё не было.
    bool setDieHeld(std::size_t dieIndex, bool held);
    /// Можно ли бросать ещё: партия идёт, не прервана, бросков меньше MAX_ROLLS.
    bool canRollAgain() const;
    /// Записывает результат в свободную графу и передаёт ход следующему игроку.
    /// category — выбранная графа; если комбинация не подходит, записывается 0.
    /// Возвращает false, если партии нет, броска не было или графа занята.
    bool commitScore(Category category);
    /// Отмечает, что игрок запросил «меню» или «выход». Программу не завершает.
    void interruptGame();
    /// Запрошен ли выход из партии.
    bool isInterrupted() const;
    /// Сыграны ли все ROUND_COUNT раундов.
    bool isGameOver() const;
    // Сколько игроков добавлено.
    std::size_t getPlayerCount() const;
    /// Индекс игрока, который сейчас ходит.
    std::size_t getCurrentPlayerIndex() const;
    /// Имя игрока в UTF-8 или nullptr при неверном индексе.
    /// Указатель на внутренний буфер, память не передаётся.
    const char* getPlayerName(std::size_t playerIndex) const;
    /// Номер раунда 1…ROUND_COUNT; 0 — партия не начата.
    int getRoundNumber() const;
    /// Сколько бросков сделано в текущем ходу, 0…MAX_ROLLS.
    int getRollNumber() const;
    /// Значение кубика 1…6; 0 при неверном индексе или до первого броска.
    int getDieValue(std::size_t dieIndex) const;
    /// Отложен ли кубик; false при неверном индексе.
    bool isDieHeld(std::size_t dieIndex) const;
    /// Свободна ли графа у текущего игрока.
    bool isCategoryFree(Category category) const;
    /// Записывает в outCategories свободные графы текущего игрока, не больше capacity.
    /// Возвращает число записанных граф; 0, если партии нет.
    std::size_t getFreeCategories(Category* outCategories, std::size_t capacity) const;
    /// Сколько очков даст текущий набор кубиков в графе. Таблицу не меняет.
    int previewScore(Category category) const;
    /// Записанное значение графы игрока; -1, если графа пуста или параметры неверны.
    int getCategoryScore(std::size_t playerIndex, Category category) const;
    /// Итог игрока с бонусом; -1 при неверном индексе.
    int getFinalScore(std::size_t playerIndex) const;
    /// Входит ли игрок в число победителей. Несколько победителей — ничья.
    /// false, если партия не завершена или индекс неверен.
    bool isWinner(std::size_t playerIndex) const;
private:
    Player m_players[MAX_PLAYERS];        ///< игроки в порядке хода
    std::size_t m_playerCount = 0;        ///< сколько ячеек массива занято
    std::size_t m_currentPlayerIndex = 0; ///< чей ход
    int m_roundNumber = 0;                ///< 0 — партии нет
    int m_rollNumber = 0;                 ///< бросков в текущем ходу
    DiceSet m_dice;                       ///< кубики текущего хода
    bool m_gameOver = false;
    bool m_interrupted = false;
};

#endif