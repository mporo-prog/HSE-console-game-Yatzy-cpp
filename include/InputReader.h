// InputReader.h — ввод с клавиатуры с проверкой.
// Ответственный: Новикова Полина (Интерфейс).
// Весь ввод в программе идёт только через этот класс: так команды «меню» и
// «выход» работают на любом шаге.
#pragma once

#include <cstddef>
#include <exception>

/// Команда, которую можно ввести на любом шаге, чтобы вернуться в меню.
constexpr const char* MENU_COMMAND = "меню";
/// Команда, которую можно ввести на любом шаге, чтобы выйти из игры.
constexpr const char* EXIT_COMMAND = "выход";
/// Те же команды латиницей — на случай, если включена английская раскладка.
constexpr const char* MENU_COMMAND_LATIN = "menu";
constexpr const char* EXIT_COMMAND_LATIN = "exit";

/// Размер буфера для одной строки ввода в байтах (вместе с '\0').
/// Русская буква занимает 2 байта, поэтому влезает около 60 русских символов.
constexpr std::size_t INPUT_BUFFER_SIZE = 128;

/// Бросается, когда игрок ввёл команду «меню». Ловится в Menu.
class MenuRequested : public std::exception
{
public:
    const char* what() const noexcept override { return "menu requested"; }
};

/// Бросается, когда игрок ввёл команду «выход» или закрыл ввод (EOF). Ловится в main.
class ExitRequested : public std::exception
{
public:
    const char* what() const noexcept override { return "exit requested"; }
};

/// Чтение строк и чисел с проверкой.
class InputReader
{
public:
    /// Показывает подсказку prompt и читает строку в buffer (массив на capacity байт).
    /// Убирает пробелы по краям. Пустая строка допустима — проверяет вызывающий код.
    /// Если строка не помещается в буфер, сообщает об ошибке и спрашивает снова.
    /// Бросает MenuRequested или ExitRequested, если введена команда или ввод закрыт.
    void readLine(const char* prompt, char* buffer, std::size_t capacity);

    /// Читает целое число от minValue до maxValue включительно.
    /// Переспрашивает, пока не введено корректное число.
    /// Бросает MenuRequested или ExitRequested, как readLine.
    int readInt(const char* prompt, int minValue, int maxValue);
};

/// Разбор введённого текста. Вынесен отдельно, чтобы его можно было проверить тестами.
namespace InputParsing
{
    /// Убирает пробелы и табуляции в начале и в конце строки (меняет саму строку).
    void trim(char* text);

    /// Сравнивает строки UTF-8 без учёта регистра.
    /// Понимает латиницу и кириллицу, включая «Ё». Другие буквы сравниваются как есть.
    bool equalsIgnoreCase(const char* left, const char* right);

    /// Переводит текст в целое число. Возвращает false, если это не число целиком
    /// (например, "12а", "", "1 2") или оно не помещается в int.
    bool parseInt(const char* text, int& result);

    /// Возвращает true, если текст — команда «меню» (в любом регистре).
    bool isMenuCommand(const char* text);

    /// Возвращает true, если текст — команда «выход» (в любом регистре).
    bool isExitCommand(const char* text);
}
