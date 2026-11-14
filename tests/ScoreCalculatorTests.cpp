// Тесты подсчёта очков (ScoreCalculator).
// Обычная программа без assert: возвращает 0, если все проверки прошли, и 1, если нет.
// Без std::vector и std::string (решение команды): тесты хранятся в обычных массивах.

#include <algorithm>
#include <array>
#include <iostream>

#include "ScoreCalculator.h"

namespace {

using Dice = std::array<int, 5>;

/// Количество граф (Ones … Chance). Когда в Category.h появится CATEGORY_COUNT, заменить на него.
constexpr std::size_t CATEGORY_COUNT_FOR_TESTS = 13;

/// Один тестовый случай: кубики, графа и ожидаемые очки.
struct TestCase {
    const char* name;
    Dice dice;
    Category category;
    int expected;
};

int g_failed = 0;
int g_passed = 0;

/// Печатает кубики в виде "[1 2 3 4 5]".
void printDice(const Dice& dice) {
    std::cout << '[';
    for (std::size_t i = 0; i < dice.size(); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << dice[i];
    }
    std::cout << ']';
}

/// Считает очки и сравнивает с ожидаемыми; при ошибке печатает подробности.
void check(const TestCase& test) {
    const int actual = ScoreCalculator::calculate(test.dice, test.category);
    if (actual == test.expected) {
        ++g_passed;
        return;
    }
    ++g_failed;
    std::cout << "ОШИБКА: " << test.name << " | " << getCategoryName(test.category) << ' ';
    printDice(test.dice);
    std::cout << " | ожидалось " << test.expected << ", получено " << actual << '\n';
}

/// Прогоняет массив тестовых случаев.
template <std::size_t N>
void checkAll(const TestCase (&tests)[N]) {
    for (std::size_t i = 0; i < N; ++i) {
        check(tests[i]);
    }
}

/// Проверяет графу на всех перестановках кубиков: порядок не должен влиять на очки.
void checkAllOrders(const char* name, Dice dice, Category category, int expected) {
    std::sort(dice.begin(), dice.end());
    do {
        check({name, dice, category, expected});
    } while (std::next_permutation(dice.begin(), dice.end()));
}

/// Верхняя часть: Единицы … Шестёрки.
void testUpperSection() {
    const TestCase tests[] = {
        {"нет единиц", {2, 3, 4, 5, 6}, Category::Ones, 0},
        {"одна единица", {1, 3, 4, 5, 6}, Category::Ones, 1},
        {"пять единиц", {1, 1, 1, 1, 1}, Category::Ones, 5},
        {"две двойки", {2, 2, 3, 4, 5}, Category::Twos, 4},
        {"нет двоек", {1, 3, 4, 5, 6}, Category::Twos, 0},
        {"три тройки", {3, 3, 3, 1, 2}, Category::Threes, 9},
        {"четыре четвёрки", {4, 4, 4, 4, 1}, Category::Fours, 16},
        {"пятёрки вразброс", {5, 1, 5, 2, 5}, Category::Fives, 15},
        {"пять шестёрок", {6, 6, 6, 6, 6}, Category::Sixes, 30},
        {"нет шестёрок", {1, 2, 3, 4, 5}, Category::Sixes, 0},
    };
    checkAll(tests);
}

/// Тройка и Каре: сумма всех кубиков или 0.
void testOfAKind() {
    const TestCase tests[] = {
        {"тройка есть", {3, 3, 3, 4, 5}, Category::ThreeOfAKind, 18},
        {"тройка вразброс", {2, 6, 2, 1, 2}, Category::ThreeOfAKind, 13},
        {"тройки нет", {1, 1, 2, 2, 3}, Category::ThreeOfAKind, 0},
        {"каре засчитывается как тройка", {4, 4, 4, 4, 2}, Category::ThreeOfAKind, 18},
        {"яцзы засчитывается как тройка", {6, 6, 6, 6, 6}, Category::ThreeOfAKind, 30},
        {"каре есть", {5, 5, 5, 5, 1}, Category::FourOfAKind, 21},
        {"каре вразброс", {2, 3, 3, 3, 3}, Category::FourOfAKind, 14},
        {"каре нет, только тройка", {5, 5, 5, 1, 2}, Category::FourOfAKind, 0},
        {"яцзы засчитывается как каре", {1, 1, 1, 1, 1}, Category::FourOfAKind, 5},
    };
    checkAll(tests);
}

/// Фул-хаус: 3 одного + 2 другого = 25. Яцзы фул-хаусом не считается (решение команды).
void testFullHouse() {
    const TestCase tests[] = {
        {"фул-хаус", {2, 2, 3, 3, 3}, Category::FullHouse, 25},
        {"фул-хаус вразброс", {6, 1, 6, 1, 6}, Category::FullHouse, 25},
        {"только тройка", {2, 2, 2, 3, 4}, Category::FullHouse, 0},
        {"две пары", {1, 1, 2, 2, 3}, Category::FullHouse, 0},
        {"каре не фул-хаус", {4, 4, 4, 4, 2}, Category::FullHouse, 0},
        {"яцзы не фул-хаус", {5, 5, 5, 5, 5}, Category::FullHouse, 0},
    };
    checkAll(tests);
    checkAllOrders("фул-хаус (все перестановки)", {3, 3, 3, 5, 5}, Category::FullHouse, 25);
}

/// Малый стрит (4 подряд = 30) и большой стрит (5 подряд = 40).
void testStraights() {
    const TestCase tests[] = {
        {"малый 1-4", {1, 2, 3, 4, 6}, Category::SmallStraight, 30},
        {"малый 2-5", {2, 3, 4, 5, 2}, Category::SmallStraight, 30},
        {"малый 3-6", {3, 4, 5, 6, 1}, Category::SmallStraight, 30},
        {"малый с повтором внутри", {1, 2, 2, 3, 4}, Category::SmallStraight, 30},
        {"большой засчитывается как малый", {1, 2, 3, 4, 5}, Category::SmallStraight, 30},
        {"разрыв: 1 2 3 5 6", {1, 2, 3, 5, 6}, Category::SmallStraight, 0},
        {"только 3 подряд", {1, 2, 3, 3, 6}, Category::SmallStraight, 0},
        {"яцзы не стрит", {4, 4, 4, 4, 4}, Category::SmallStraight, 0},
        {"большой 1-5", {1, 2, 3, 4, 5}, Category::LargeStraight, 40},
        {"большой 2-6", {2, 3, 4, 5, 6}, Category::LargeStraight, 40},
        {"малый не большой", {1, 2, 3, 4, 6}, Category::LargeStraight, 0},
        {"с повтором не большой", {2, 3, 4, 5, 5}, Category::LargeStraight, 0},
        {"1 3 4 5 6 не большой", {1, 3, 4, 5, 6}, Category::LargeStraight, 0},
    };
    checkAll(tests);
    checkAllOrders("малый 3-6 (все перестановки)", {3, 4, 5, 6, 1}, Category::SmallStraight, 30);
    checkAllOrders("малый 1-4 с повтором (все перестановки)", {1, 2, 3, 4, 4}, Category::SmallStraight, 30);
    checkAllOrders("большой 1-5 (все перестановки)", {1, 2, 3, 4, 5}, Category::LargeStraight, 40);
    checkAllOrders("большой 2-6 (все перестановки)", {2, 3, 4, 5, 6}, Category::LargeStraight, 40);
}

/// Яцзы (5 одинаковых = 50) и Шанс (сумма всех).
void testYatzyAndChance() {
    const TestCase tests[] = {
        {"яцзы единицы", {1, 1, 1, 1, 1}, Category::Yatzee, 50},
        {"яцзы шестёрки", {6, 6, 6, 6, 6}, Category::Yatzee, 50},
        {"каре не яцзы", {6, 6, 6, 6, 5}, Category::Yatzee, 0},
        {"разные не яцзы", {1, 2, 3, 4, 5}, Category::Yatzee, 0},
        {"шанс минимум", {1, 1, 1, 1, 1}, Category::Chance, 5},
        {"шанс максимум", {6, 6, 6, 6, 6}, Category::Chance, 30},
        {"шанс смешанный", {1, 3, 4, 5, 6}, Category::Chance, 19},
    };
    checkAll(tests);
}

/// Кубики вне диапазона 1…6: в любой графе должно получаться 0,
/// а не выход за границы массива подсчёта значений.
void testInvalidDice() {
    const Dice invalidDice[] = {
        {0, 1, 2, 3, 4},      // ноль
        {7, 1, 2, 3, 4},      // на единицу больше максимума
        {1, 2, 3, 4, -1},     // отрицательное значение
        {100, 1, 1, 1, 1},    // далеко за границей
        {0, 0, 0, 0, 0},      // все нули (иначе было бы похоже на яцзы)
        {7, 7, 7, 7, 7},      // все семёрки
        {2, 3, 4, 5, 7},      // почти большой стрит
        {6, 6, 6, 6, 7},      // почти яцзы
    };
    for (std::size_t d = 0; d < std::size(invalidDice); ++d) {
        for (std::size_t c = 0; c < CATEGORY_COUNT_FOR_TESTS; ++c) {
            check({"кубик вне диапазона 1…6", invalidDice[d], static_cast<Category>(c), 0});
        }
    }
}

}  // namespace

int main() {
    testUpperSection();
    testOfAKind();
    testFullHouse();
    testStraights();
    testYatzyAndChance();
    testInvalidDice();

    std::cout << "ScoreCalculator: пройдено " << g_passed << ", ошибок " << g_failed << '\n';
    return g_failed == 0 ? 0 : 1;
}
