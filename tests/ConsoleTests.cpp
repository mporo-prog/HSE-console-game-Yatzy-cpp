// ConsoleTests.cpp — ширина строк UTF-8 и чтение строки.
// Ответственный: Новикова Полина (Интерфейс).
#include <string_view>

#include "Console.h"
#include "TestHelpers.h"

namespace
{
    void testDisplayWidth()
    {
        check(Console::getDisplayWidth("") == 0, "ширина пустой строки = 0");
        check(Console::getDisplayWidth("Yatzy") == 5, "ширина латиницы = числу букв");
        check(Console::getDisplayWidth("Яцзы") == 4, "русская буква = 1 символ, а не 2 байта");
        check(Console::getDisplayWidth("Каре 4") == 6, "смешанная строка");
        check(Console::getDisplayWidth("Ёж ё") == 4, "буква Ё");
        check(Console::getDisplayWidth("│─┼") == 3, "символы рамки таблицы (3 байта) = 1 символ");
    }

    void testReadLine()
    {
        char buffer[16];
        {
            ScopedConsole console("Полина\n");
            check(Console::readLine(buffer, sizeof buffer) == ReadStatus::Ok, "кириллица читается");
            check(std::string_view(buffer) == "Полина", "кириллица без изменений");
        }
        {
            ScopedConsole console("abc\r\n");
            Console::readLine(buffer, sizeof buffer);
            check(std::string_view(buffer) == "abc", "\\r в конце строки убирается");
        }
        {
            ScopedConsole console("конец");   // без \n в конце файла
            check(Console::readLine(buffer, sizeof buffer) == ReadStatus::Ok, "строка без \\n");
            check(std::string_view(buffer) == "конец", "строка без \\n читается целиком");
            check(Console::readLine(buffer, sizeof buffer) == ReadStatus::EndOfInput, "после неё — конец ввода");
        }
        {
            ScopedConsole console("");
            check(Console::readLine(buffer, sizeof buffer) == ReadStatus::EndOfInput, "пустой ввод = конец ввода");
        }
        {
            ScopedConsole console("\n");
            check(Console::readLine(buffer, sizeof buffer) == ReadStatus::Ok, "пустая строка — это не конец ввода");
            check(buffer[0] == '\0', "пустая строка");
        }
        {
            // 15 байт помещается (16 с '\0'), 16 — уже нет
            ScopedConsole console("123456789012345\n1234567890123456\nдальше\n");
            check(Console::readLine(buffer, sizeof buffer) == ReadStatus::Ok, "ровно по размеру буфера");
            check(Console::readLine(buffer, sizeof buffer) == ReadStatus::TooLong, "на 1 байт длиннее буфера");
            check(buffer[0] == '\0', "после TooLong буфер пустой");
            check(Console::readLine(buffer, sizeof buffer) == ReadStatus::Ok, "после TooLong следующая строка читается");
            check(std::string_view(buffer) == "дальше", "остаток длинной строки пропущен");
        }
    }

    void testColorsOffWithoutInitialize()
    {
        // Без initialize (и когда вывод не в консоль) цвета выключены
        check(!Console::isColorEnabled(), "по умолчанию цвета выключены");
        Console::setColorEnabled(true);
        check(!Console::isColorEnabled(), "нельзя включить цвета, если консоль их не поддерживает");
    }
}

int main()
{
    testDisplayWidth();
    testReadLine();
    testColorsOffWithoutInitialize();
    return reportResults("ConsoleTests");
}
