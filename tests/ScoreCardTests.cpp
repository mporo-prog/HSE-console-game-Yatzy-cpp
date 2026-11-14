// Тесты таблицы очков (ScoreCard).
// Обычная программа без assert: возвращает 0, если все проверки прошли, и 1, если нет.
// Без std::vector и std::string (решение команды).

#include <cstddef>
#include <iostream>

#include "ScoreCard.h"

namespace {

/// Количество граф (Ones … Chance). Когда в Category.h появится CATEGORY_COUNT, заменить на него.
constexpr std::size_t CATEGORY_COUNT_FOR_TESTS = 13;

int g_failed = 0;
int g_passed = 0;

/// Сравнивает число с ожидаемым; при ошибке печатает название проверки.
void checkEqual(const char* name, int actual, int expected) {
    if (actual == expected) {
        ++g_passed;
        return;
    }
    ++g_failed;
    std::cout << "ОШИБКА: " << name << " | ожидалось " << expected << ", получено " << actual << '\n';
}

/// Сравнивает логическое значение с ожидаемым.
void checkBool(const char* name, bool actual, bool expected) {
    checkEqual(name, actual ? 1 : 0, expected ? 1 : 0);
}

/// Возвращает графу по номеру (0 = Ones … 12 = Chance).
Category categoryAt(std::size_t index) {
    return static_cast<Category>(index);
}

/// Записывает очки в графы верхней части: scores[0] — Единицы, …, scores[5] — Шестёрки.
void fillUpper(ScoreCard& card, const int (&scores)[6]) {
    for (std::size_t i = 0; i < 6; ++i) {
        card.setScore(categoryAt(i), scores[i]);
    }
}

/// Новая таблица: все графы пустые, итог 0.
void testEmptyCard() {
    const ScoreCard card;
    for (std::size_t i = 0; i < CATEGORY_COUNT_FOR_TESTS; ++i) {
        checkBool("новая таблица: графа пустая", card.isFilled(categoryAt(i)), false);
        checkEqual("новая таблица: пустая графа возвращает -1", card.getScore(categoryAt(i)), -1);
    }
    checkEqual("новая таблица: бонус 0", card.getUpperBonus(), 0);
    checkEqual("новая таблица: итог 0", card.getTotalScore(), 0);
}

/// Запись в свободную графу.
void testSetScore() {
    ScoreCard card;
    checkBool("запись в свободную графу удаётся", card.setScore(Category::Fives, 15), true);
    checkBool("после записи графа занята", card.isFilled(Category::Fives), true);
    checkEqual("после записи графа хранит очки", card.getScore(Category::Fives), 15);
    checkBool("соседняя графа осталась пустой", card.isFilled(Category::Sixes), false);
    checkEqual("соседняя графа возвращает -1", card.getScore(Category::Sixes), -1);

    // 0 очков — это заполненная графа, а не пустая
    checkBool("запись 0 очков удаётся", card.setScore(Category::Yatzee, 0), true);
    checkBool("графа с 0 очков занята", card.isFilled(Category::Yatzee), true);
    checkEqual("графа с 0 очков возвращает 0, а не -1", card.getScore(Category::Yatzee), 0);
}

/// Запись в занятую графу и отрицательные очки.
void testRejectedScores() {
    ScoreCard card;
    card.setScore(Category::FullHouse, 25);
    checkBool("повторная запись в занятую графу отклоняется", card.setScore(Category::FullHouse, 0), false);
    checkEqual("занятая графа сохраняет первое значение", card.getScore(Category::FullHouse), 25);

    card.setScore(Category::Chance, 0);
    checkBool("графа с 0 очков тоже не перезаписывается", card.setScore(Category::Chance, 30), false);
    checkEqual("графа с 0 очков сохраняет 0", card.getScore(Category::Chance), 0);

    checkBool("отрицательные очки отклоняются", card.setScore(Category::Ones, -5), false);
    checkBool("после отказа графа осталась пустой", card.isFilled(Category::Ones), false);
    checkEqual("после отказа итог не изменился", card.getTotalScore(), 25);
}

/// Бонус верхней части: +35 при сумме 63 и больше.
void testUpperBonus() {
    ScoreCard exactly63;
    fillUpper(exactly63, {3, 6, 9, 12, 15, 18});  // по три кубика каждого значения = 63
    checkEqual("сумма ровно 63: бонус 35", exactly63.getUpperBonus(), 35);
    checkEqual("сумма ровно 63: итог 63 + 35", exactly63.getTotalScore(), 98);

    ScoreCard sum62;
    fillUpper(sum62, {2, 6, 9, 12, 15, 18});
    checkEqual("сумма 62: бонуса нет", sum62.getUpperBonus(), 0);
    checkEqual("сумма 62: итог без бонуса", sum62.getTotalScore(), 62);

    ScoreCard sum64;
    fillUpper(sum64, {4, 6, 9, 12, 15, 18});
    checkEqual("сумма 64: бонус 35", sum64.getUpperBonus(), 35);

    ScoreCard partial;
    partial.setScore(Category::Fives, 25);
    partial.setScore(Category::Sixes, 30);
    partial.setScore(Category::Fours, 8);
    checkEqual("бонус засчитывается, даже если заполнена не вся верхняя часть", partial.getUpperBonus(), 35);

    ScoreCard lowerOnly;
    lowerOnly.setScore(Category::Yatzee, 50);
    lowerOnly.setScore(Category::LargeStraight, 40);
    checkEqual("нижняя часть не даёт бонус верхней", lowerOnly.getUpperBonus(), 0);
    checkEqual("нижняя часть: итог без бонуса", lowerOnly.getTotalScore(), 90);
}

/// Итоговая сумма по всей таблице.
void testTotalScore() {
    ScoreCard card;
    fillUpper(card, {3, 6, 9, 12, 15, 18});        // 63 + бонус 35
    card.setScore(Category::ThreeOfAKind, 20);
    card.setScore(Category::FourOfAKind, 0);
    card.setScore(Category::FullHouse, 25);
    card.setScore(Category::SmallStraight, 30);
    card.setScore(Category::LargeStraight, 40);
    card.setScore(Category::Yatzee, 50);
    card.setScore(Category::Chance, 22);
    // 63 + 35 + 20 + 0 + 25 + 30 + 40 + 50 + 22 = 285
    checkEqual("полная таблица: итог", card.getTotalScore(), 285);

    for (std::size_t i = 0; i < CATEGORY_COUNT_FOR_TESTS; ++i) {
        checkBool("полная таблица: все графы заняты", card.isFilled(categoryAt(i)), true);
    }

    ScoreCard maxCard;
    fillUpper(maxCard, {5, 10, 15, 20, 25, 30});    // 105 + бонус 35
    maxCard.setScore(Category::ThreeOfAKind, 30);
    maxCard.setScore(Category::FourOfAKind, 30);
    maxCard.setScore(Category::FullHouse, 25);
    maxCard.setScore(Category::SmallStraight, 30);
    maxCard.setScore(Category::LargeStraight, 40);
    maxCard.setScore(Category::Yatzee, 50);
    maxCard.setScore(Category::Chance, 30);
    // 105 + 35 + 30 + 30 + 25 + 30 + 40 + 50 + 30 = 375 — максимум в нашей версии правил
    checkEqual("максимальный итог 375", maxCard.getTotalScore(), 375);
}

}  // namespace

int main() {
    testEmptyCard();
    testSetScore();
    testRejectedScores();
    testUpperBonus();
    testTotalScore();

    std::cout << "ScoreCard: пройдено " << g_passed << ", ошибок " << g_failed << '\n';
    return g_failed == 0 ? 0 : 1;
}
