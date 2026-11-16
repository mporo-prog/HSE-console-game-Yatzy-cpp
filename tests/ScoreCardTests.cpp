
#include <cstddef>
#include <iostream>

#include "ScoreCard.h"

namespace {

int g_failed = 0;
int g_passed = 0;

/// Сравнивает число с ожидаемым значением.
void checkEqual(const char* name, int actual, int expected) {
    if (actual == expected) {
        ++g_passed;
        return;
    }

    ++g_failed;
    std::cout << "ОШИБКА: " << name
              << " | ожидалось " << expected
              << ", получено " << actual << '\n';
}

/// Сравнивает логические значения.
void checkBool(const char* name, bool actual, bool expected) {
    checkEqual(name, actual ? 1 : 0, expected ? 1 : 0);
}

/// Возвращает категорию по индексу.
Category categoryAt(std::size_t index) {
    return static_cast<Category>(index);
}

/// Заполняет шесть категорий верхней части.
void fillUpper(
    ScoreCard& card,
    const int (&scores)[UPPER_CATEGORY_COUNT]
) {
    for (std::size_t i = 0; i < UPPER_CATEGORY_COUNT; ++i) {
        card.setScore(categoryAt(i), scores[i]);
    }
}

/// Проверяет новую пустую таблицу.
void testEmptyCard() {
    const ScoreCard card;

    for (std::size_t i = 0; i < CATEGORY_COUNT; ++i) {
        checkBool(
            "новая таблица: графа пустая",
            card.isFilled(categoryAt(i)),
            false
        );

        checkEqual(
            "новая таблица: пустая графа возвращает -1",
            card.getScore(categoryAt(i)),
            -1
        );
    }

    checkEqual("новая таблица: верхняя сумма 0",
               card.getUpperSum(), 0);

    checkEqual("новая таблица: бонус 0",
               card.getUpperBonus(), 0);

    checkEqual("новая таблица: итог 0",
               card.getTotalScore(), 0);

    checkBool("новая таблица не завершена",
              card.isComplete(), false);
}

/// Проверяет запись очков.
void testSetScore() {
    ScoreCard card;

    checkBool(
        "запись в свободную графу удаётся",
        card.setScore(Category::Fives, 15),
        true
    );

    checkBool(
        "после записи графа занята",
        card.isFilled(Category::Fives),
        true
    );

    checkEqual(
        "после записи графа хранит очки",
        card.getScore(Category::Fives),
        15
    );

    checkBool(
        "соседняя графа осталась пустой",
        card.isFilled(Category::Sixes),
        false
    );

    checkEqual(
        "соседняя графа возвращает -1",
        card.getScore(Category::Sixes),
        -1
    );

    checkBool(
        "запись 0 очков удаётся",
        card.setScore(Category::Yatzy, 0),
        true
    );

    checkBool(
        "графа с 0 очков занята",
        card.isFilled(Category::Yatzy),
        true
    );

    checkEqual(
        "графа с 0 очков возвращает 0",
        card.getScore(Category::Yatzy),
        0
    );
}

/// Проверяет повторную запись и отрицательные очки.
void testRejectedScores() {
    ScoreCard card;

    card.setScore(Category::FullHouse, 25);

    checkBool(
        "повторная запись отклоняется",
        card.setScore(Category::FullHouse, 0),
        false
    );

    checkEqual(
        "первая запись сохраняется",
        card.getScore(Category::FullHouse),
        25
    );

    card.setScore(Category::Chance, 0);

    checkBool(
        "графа с нулём не перезаписывается",
        card.setScore(Category::Chance, 30),
        false
    );

    checkEqual(
        "графа сохраняет 0",
        card.getScore(Category::Chance),
        0
    );

    checkBool(
        "отрицательные очки отклоняются",
        card.setScore(Category::Ones, -5),
        false
    );

    checkBool(
        "после отказа графа пустая",
        card.isFilled(Category::Ones),
        false
    );

    checkEqual(
        "итог после отказа не изменился",
        card.getTotalScore(),
        25
    );
}

/// Проверяет бонус верхней части.
void testUpperBonus() {
    ScoreCard exactly63;

    fillUpper(exactly63, {3, 6, 9, 12, 15, 18});

    checkEqual("сумма ровно 63",
               exactly63.getUpperSum(), 63);

    checkEqual("бонус за 63",
               exactly63.getUpperBonus(), 35);

    checkEqual("итог 63 + 35",
               exactly63.getTotalScore(), 98);

    ScoreCard sum62;

    fillUpper(sum62, {2, 6, 9, 12, 15, 18});

    checkEqual("сумма 62",
               sum62.getUpperSum(), 62);

    checkEqual("бонус за 62 отсутствует",
               sum62.getUpperBonus(), 0);

    checkEqual("итог без бонуса",
               sum62.getTotalScore(), 62);

    ScoreCard sum64;

    fillUpper(sum64, {4, 6, 9, 12, 15, 18});

    checkEqual("сумма 64",
               sum64.getUpperSum(), 64);

    checkEqual("бонус за 64",
               sum64.getUpperBonus(), 35);

    ScoreCard partial;

    partial.setScore(Category::Fives, 25);
    partial.setScore(Category::Sixes, 30);
    partial.setScore(Category::Fours, 8);

    checkEqual(
        "бонус при частично заполненной верхней части",
        partial.getUpperBonus(),
        35
    );

    ScoreCard lowerOnly;

    lowerOnly.setScore(Category::Yatzy, 50);
    lowerOnly.setScore(Category::LargeStraight, 40);

    checkEqual(
        "нижняя часть не даёт верхний бонус",
        lowerOnly.getUpperBonus(),
        0
    );

    checkEqual(
        "нижняя часть: итог 90",
        lowerOnly.getTotalScore(),
        90
    );
}

/// Проверяет итоговую сумму таблицы.
void testTotalScore() {
    ScoreCard card;

    fillUpper(card, {3, 6, 9, 12, 15, 18});

    card.setScore(Category::ThreeOfAKind, 20);
    card.setScore(Category::FourOfAKind, 0);
    card.setScore(Category::FullHouse, 25);
    card.setScore(Category::SmallStraight, 30);
    card.setScore(Category::LargeStraight, 40);
    card.setScore(Category::Yatzy, 50);
    card.setScore(Category::Chance, 22);

    checkEqual(
        "полная таблица: итог 285",
        card.getTotalScore(),
        285
    );

    for (std::size_t i = 0; i < CATEGORY_COUNT; ++i) {
        checkBool(
            "полная таблица: графа занята",
            card.isFilled(categoryAt(i)),
            true
        );
    }

    checkBool(
        "полная таблица завершена",
        card.isComplete(),
        true
    );

    ScoreCard maxCard;

    fillUpper(maxCard, {5, 10, 15, 20, 25, 30});

    maxCard.setScore(Category::ThreeOfAKind, 30);
    maxCard.setScore(Category::FourOfAKind, 30);
    maxCard.setScore(Category::FullHouse, 25);
    maxCard.setScore(Category::SmallStraight, 30);
    maxCard.setScore(Category::LargeStraight, 40);
    maxCard.setScore(Category::Yatzy, 50);
    maxCard.setScore(Category::Chance, 30);

    checkEqual(
        "максимальный итог 375",
        maxCard.getTotalScore(),
        375
    );

    checkBool(
        "максимальная таблица завершена",
        maxCard.isComplete(),
        true
    );
}

/// Проверяет заполненность таблицы.
void testIsComplete() {
    ScoreCard card;

    checkBool(
        "пустая таблица не завершена",
        card.isComplete(),
        false
    );

    for (std::size_t i = 0; i < CATEGORY_COUNT - 1; ++i) {
        card.setScore(categoryAt(i), 0);
    }

    checkBool(
        "одна пустая категория",
        card.isComplete(),
        false
    );

    card.setScore(Category::Chance, 0);

    checkBool(
        "все категории заполнены",
        card.isComplete(),
        true
    );
}

} // namespace

int main() {
    testEmptyCard();
    testSetScore();
    testRejectedScores();
    testUpperBonus();
    testTotalScore();
    testIsComplete();

    std::cout << "ScoreCard: пройдено "
              << g_passed
              << ", ошибок "
              << g_failed << '\n';

    return g_failed == 0 ? 0 : 1;
}
