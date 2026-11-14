// TextTests.cpp — длина строки и копирование с обрезкой по границе буквы UTF-8.
// Ответственный: Новикова Полина (Интерфейс).
#include <string_view>

#include "Text.h"
#include "TestHelpers.h"

namespace
{
    void testLength()
    {
        check(Text::length("") == 0, "длина пустой строки = 0");
        check(Text::length(nullptr) == 0, "длина nullptr = 0");
        check(Text::length("Yatzy") == 5, "латиница: 1 байт на букву");
        check(Text::length("Яцзы") == 8, "кириллица: 2 байта на букву");
    }

    void testCopyFits()
    {
        char buffer[16];
        check(Text::copy(buffer, sizeof buffer, "Полина"), "русское имя помещается — true");
        check(std::string_view(buffer) == "Полина", "русское имя скопировано без изменений");

        check(Text::copy(buffer, sizeof buffer, "123456789012345"), "ровно 15 байт в буфер на 16 — true");
        check(std::string_view(buffer) == "123456789012345", "ровно по размеру буфера");

        check(Text::copy(buffer, sizeof buffer, ""), "пустая строка — true");
        check(buffer[0] == '\0', "пустая строка копируется как пустая");

        check(Text::copy(buffer, sizeof buffer, nullptr), "nullptr — true");
        check(buffer[0] == '\0', "nullptr считается пустой строкой");
    }

    void testCopyTruncated()
    {
        char buffer[8];   // 7 байт + '\0'

        check(!Text::copy(buffer, sizeof buffer, "abcdefghij"), "латиница не помещается — false");
        check(std::string_view(buffer) == "abcdefg", "латиница обрезается до 7 байт");

        // «Полина» = 12 байт; 7 байт = 3 буквы и половина четвёртой
        check(!Text::copy(buffer, sizeof buffer, "Полина"), "кириллица не помещается — false");
        check(std::string_view(buffer) == "Пол", "половина буквы не копируется");

        // a + 3 русские буквы = 7 байт ровно, дальше ещё буква
        check(!Text::copy(buffer, sizeof buffer, "aБВГД"), "граница буквы совпала с концом буфера — false");
        check(std::string_view(buffer) == "aБВГ", "обрезка ровно по границе буквы");

        // символ рамки «│» = 3 байта: 2 символа (6 байт) + 2 байта третьего не помещаются
        check(!Text::copy(buffer, sizeof buffer, "│││"), "трёхбайтовые символы — false");
        check(std::string_view(buffer) == "││", "трёхбайтовый символ не разрезается");
    }

    void testCopyEdgeCases()
    {
        char one[1] = {'x'};
        check(!Text::copy(one, sizeof one, "a"), "буфер на 1 байт и непустая строка — false");
        check(one[0] == '\0', "в буфере на 1 байт остаётся только '\\0'");

        char tiny[2];
        check(!Text::copy(tiny, sizeof tiny, "Я"), "русская буква в буфер на 2 байта — false");
        check(tiny[0] == '\0', "русская буква в буфер на 2 байта не помещается совсем");

        char buffer[4] = {'x', 'x', 'x', 'x'};
        check(!Text::copy(buffer, 0, "a"), "destSize = 0 — false");
        check(buffer[0] == 'x', "при destSize = 0 буфер не меняется");
        check(!Text::copy(nullptr, 4, "a"), "dest = nullptr — false");
    }
}

int main()
{
    testLength();
    testCopyFits();
    testCopyTruncated();
    testCopyEdgeCases();
    return reportResults("TextTests");
}
