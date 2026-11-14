// InputReader.cpp — чтение ввода и обработка команд «меню» и «выход».
#include "InputReader.h"

#include <charconv>
#include <iostream>
#include <stdexcept>
#include <system_error>

#include "Console.h"

namespace
{
    bool isSpace(char ch)
    {
        return ch == ' ' || ch == '\t';
    }

    /// Печатает сообщение об ошибке красным и переводит строку.
    void printError(const char* message)
    {
        Console::printColored(message, ConsoleColor::Red);
        std::cout << '\n';
    }

    /// Читает один символ из text начиная с позиции index и сдвигает index на следующий.
    /// Возвращает код символа в нижнем регистре: так «А» и «а» дают одно и то же число.
    ///
    /// Русские буквы в UTF-8 — два байта, их склеиваем в одно число:
    ///   А–П: D0 90 … D0 9F  ->  а–п: D0 B0 … D0 BF  (второй байт + 0x20)
    ///   Р–Я: D0 A0 … D0 AF  ->  р–я: D1 80 … D1 8F
    ///   Ё:   D0 81          ->  ё:   D1 91
    unsigned readLowerChar(const char* text, std::size_t& index)
    {
        const auto byte = static_cast<unsigned>(static_cast<unsigned char>(text[index]));
        if (byte >= 'A' && byte <= 'Z')
        {
            ++index;
            return byte + ('a' - 'A');
        }

        const auto next = static_cast<unsigned>(static_cast<unsigned char>(text[index + 1]));
        const bool isCyrillic = (byte == 0xD0u || byte == 0xD1u) && (next & 0xC0u) == 0x80u;
        if (!isCyrillic)
        {
            ++index;
            return byte;
        }

        index += 2;
        const unsigned code = (byte << 8u) | next;
        if (code >= 0xD090u && code <= 0xD09Fu)
        {
            return code + 0x20u;
        }
        if (code >= 0xD0A0u && code <= 0xD0AFu)
        {
            return code - 0xD0A0u + 0xD180u;
        }
        if (code == 0xD081u)
        {
            return 0xD191u;
        }
        return code;
    }

    /// Бросает исключение, если в строке команда «меню» или «выход».
    void throwIfCommand(const char* text)
    {
        if (InputParsing::isMenuCommand(text))
        {
            throw MenuRequested();
        }
        if (InputParsing::isExitCommand(text))
        {
            throw ExitRequested();
        }
    }
}

namespace InputParsing
{
    void trim(char* text)
    {
        std::size_t start = 0;
        while (isSpace(text[start]))
        {
            ++start;
        }

        std::size_t length = 0;
        while (text[start + length] != '\0')
        {
            ++length;
        }
        while (length > 0 && isSpace(text[start + length - 1]))
        {
            --length;
        }

        for (std::size_t i = 0; i < length; ++i)
        {
            text[i] = text[start + i];
        }
        text[length] = '\0';
    }

    bool equalsIgnoreCase(const char* left, const char* right)
    {
        std::size_t leftIndex = 0;
        std::size_t rightIndex = 0;
        while (left[leftIndex] != '\0' && right[rightIndex] != '\0')
        {
            if (readLowerChar(left, leftIndex) != readLowerChar(right, rightIndex))
            {
                return false;
            }
        }
        return left[leftIndex] == '\0' && right[rightIndex] == '\0';
    }

    bool parseInt(const char* text, int& result)
    {
        std::size_t length = 0;
        while (text[length] != '\0')
        {
            ++length;
        }
        if (length == 0)
        {
            return false;
        }

        int value = 0;
        const char* end = text + length;
        const std::from_chars_result parsed = std::from_chars(text, end, value);
        if (parsed.ec != std::errc() || parsed.ptr != end)
        {
            return false;
        }
        result = value;
        return true;
    }

    bool isMenuCommand(const char* text)
    {
        return equalsIgnoreCase(text, MENU_COMMAND) || equalsIgnoreCase(text, MENU_COMMAND_LATIN);
    }

    bool isExitCommand(const char* text)
    {
        return equalsIgnoreCase(text, EXIT_COMMAND) || equalsIgnoreCase(text, EXIT_COMMAND_LATIN);
    }
}

void InputReader::readLine(const char* prompt, char* buffer, std::size_t capacity)
{
    while (true)
    {
        std::cout << prompt;
        const ReadStatus status = Console::readLine(buffer, capacity);

        if (status == ReadStatus::EndOfInput)
        {
            // Ввод закрыт (Ctrl+D на Mac, Ctrl+Z на Windows) — выходим из игры
            std::cout << '\n';
            throw ExitRequested();
        }
        if (status == ReadStatus::TooLong)
        {
            printError("Слишком длинная строка. Попробуйте короче.");
            continue;
        }

        InputParsing::trim(buffer);
        throwIfCommand(buffer);
        return;
    }
}

int InputReader::readInt(const char* prompt, int minValue, int maxValue)
{
    if (minValue > maxValue)
    {
        throw std::invalid_argument("InputReader::readInt: minValue > maxValue");
    }

    char buffer[INPUT_BUFFER_SIZE];
    while (true)
    {
        readLine(prompt, buffer, INPUT_BUFFER_SIZE);

        int value = 0;
        if (!InputParsing::parseInt(buffer, value))
        {
            printError("Нужно ввести целое число.");
            continue;
        }
        if (value < minValue || value > maxValue)
        {
            Console::setColor(ConsoleColor::Red);
            std::cout << "Введите число от " << minValue << " до " << maxValue << '.';
            Console::resetColor();
            std::cout << '\n';
            continue;
        }
        return value;
    }
}
