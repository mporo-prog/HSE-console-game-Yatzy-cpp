#include "../include/Game.h"

Game::Game() = default;

bool Game::addPlayer(const char* /*name*/) {
    return false;
}

bool Game::startNewGame() {
    return false;
}

void Game::reset() {
    m_playerCount = 0;
    m_currentPlayerIndex = 0;
    m_roundNumber = 0;
    m_rollNumber = 0;
    m_gameOver = false;
    m_interrupted = false;
}

bool Game::rollDice() {
    return false;
}

bool Game::setDieHeld(std::size_t /*dieIndex*/, bool /*held*/) {
    return false;
}

bool Game::canRollAgain() const {
    return false;
}

bool Game::commitScore(Category /*category*/) {
    return false;
}

void Game::interruptGame() {
    m_interrupted = true;
}

bool Game::isInterrupted() const {
    return m_interrupted;
}

bool Game::isGameOver() const {
    return m_gameOver;
}

std::size_t Game::getPlayerCount() const {
    return m_playerCount;
}

std::size_t Game::getCurrentPlayerIndex() const {
    return m_currentPlayerIndex;
}

const char* Game::getPlayerName(std::size_t /*playerIndex*/) const {
    return nullptr;
}

int Game::getRoundNumber() const {
    return m_roundNumber;
}

int Game::getRollNumber() const {
    return m_rollNumber;
}

int Game::getDieValue(std::size_t /*dieIndex*/) const {
    return 0;
}

bool Game::isDieHeld(std::size_t /*dieIndex*/) const {
    return false;
}

bool Game::isCategoryFree(Category /*category*/) const {
    return false;
}

std::size_t Game::getFreeCategories(Category* /*outCategories*/, std::size_t /*capacity*/) const {
    return 0;
}

int Game::previewScore(Category /*category*/) const {
    return 0;
}

int Game::getCategoryScore(std::size_t /*playerIndex*/, Category /*category*/) const {
    return -1;
}

int Game::getFinalScore(std::size_t /*playerIndex*/) const {
    return -1;
}

bool Game::isWinner(std::size_t /*playerIndex*/) const {
    return false;
}