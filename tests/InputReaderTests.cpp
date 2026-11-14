// InputReaderTests.cpp — разбор ввода, команды «меню»/«выход», повторный вопрос.
// Ответственный: Новикова Полина (Интерфейс).
#include <string_view>

#include "InputReader.h"
#include "TestHelpers.h"

namespace
{
    void testTrim()
    {
        char text1[] = "  Полина \t";
        InputParsing::trim(text1);
        check(std::string_view(text1) == "Полина", "пробелы по краям убираются");

        char text2[] = "   ";
        InputParsing::trim(text2);
        check(text2[0] == '\0', "строка из пробелов становится пустой");

        char text3[] = "Анна Мария";
        InputParsing::trim(text3);
        check(std::string_view(text3) == "Анна Мария", "пробел внутри остаётся");
    }

    void testCommands()
    {
        check(InputParsing::isMenuCommand("меню"), "меню");
        check(InputParsing::isMenuCommand("МЕНЮ"), "МЕНЮ");
        check(InputParsing::isMenuCommand("Меню"), "Меню");
        check(InputParsing::isMenuCommand("MeNu"), "menu латиницей");
        check(InputParsing::isExitCommand("Выход"), "Выход");
        check(InputParsing::isExitCommand("ВЫХОД"), "ВЫХОД");
        check(InputParsing::isExitCommand("exit"), "exit");
        check(!InputParsing::isMenuCommand("мен"), "часть слова — не команда");
        check(!InputParsing::isMenuCommand("меню2"), "слово с хвостом — не команда");
        check(!InputParsing::isExitCommand(""), "пустая строка — не команда");
        check(InputParsing::equalsIgnoreCase("ЁЛКА", "ёлка"), "Ё и ё");
        check(InputParsing::equalsIgnoreCase("АБВГДЕЖЗИЙКЛМНОП", "абвгдежзийклмноп"), "А–П");
        check(InputParsing::equalsIgnoreCase("РСТУФХЦЧШЩЪЫЬЭЮЯ", "рстуфхцчшщъыьэюя"), "Р–Я");
    }

    void testParseInt()
    {
        int value = 0;
        check(InputParsing::parseInt("5", value) && value == 5, "5");
        check(InputParsing::parseInt("-3", value) && value == -3, "-3");
        check(!InputParsing::parseInt("", value), "пустая строка — не число");
        check(!InputParsing::parseInt("12а", value), "12а — не число");
        check(!InputParsing::parseInt("1 2", value), "1 2 — не число");
        check(!InputParsing::parseInt("пять", value), "пять — не число");
        check(!InputParsing::parseInt("99999999999", value), "не помещается в int");
    }

    void testReadLine()
    {
        InputReader reader;
        char buffer[INPUT_BUFFER_SIZE];
        {
            ScopedConsole console("  Дима  \n");
            reader.readLine("> ", buffer, INPUT_BUFFER_SIZE);
            check(std::string_view(buffer) == "Дима", "readLine обрезает пробелы");
        }
        {
            ScopedConsole console(" МЕНЮ \n");
            bool thrown = false;
            try { reader.readLine("> ", buffer, INPUT_BUFFER_SIZE); }
            catch (const MenuRequested&) { thrown = true; }
            check(thrown, "«МЕНЮ» с пробелами бросает MenuRequested");
        }
        {
            ScopedConsole console("выход\n");
            bool thrown = false;
            try { reader.readLine("> ", buffer, INPUT_BUFFER_SIZE); }
            catch (const ExitRequested&) { thrown = true; }
            check(thrown, "«выход» бросает ExitRequested");
        }
        {
            ScopedConsole console("");
            bool thrown = false;
            try { reader.readLine("> ", buffer, INPUT_BUFFER_SIZE); }
            catch (const ExitRequested&) { thrown = true; }
            check(thrown, "конец ввода бросает ExitRequested");
        }
        {
            char small[8];
            ScopedConsole console("очень длинное имя\nЛев\n");
            reader.readLine("> ", small, sizeof small);
            check(std::string_view(small) == "Лев", "слишком длинную строку переспрашивает");
        }
    }

    void testReadInt()
    {
        InputReader reader;
        {
            ScopedConsole console("abc\n0\n7\n 3 \n");
            check(reader.readInt("> ", 1, 6) == 3, "readInt переспрашивает до числа в диапазоне");
        }
        {
            ScopedConsole console("1\n");
            check(reader.readInt("> ", 1, 1) == 1, "диапазон из одного числа");
        }
        {
            ScopedConsole console("5\nменю\n2\n");
            bool thrown = false;
            try { reader.readInt("> ", 1, 3); }
            catch (const MenuRequested&) { thrown = true; }
            check(thrown, "команда «меню» работает и внутри readInt");
        }
        {
            ScopedConsole console("abc\n");
            bool thrown = false;
            try { reader.readInt("> ", 1, 6); }
            catch (const ExitRequested&) { thrown = true; }
            check(thrown, "конец ввода внутри readInt — выход, а не бесконечный цикл");
        }
    }
}

int main()
{
    testTrim();
    testCommands();
    testParseInt();
    testReadLine();
    testReadInt();
    return reportResults("InputReaderTests");
}
