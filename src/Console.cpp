// Console.cpp — единственное место в проекте, где есть код под конкретную ОС.
#include "Console.h"

#include <iostream>
#include <string_view>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
// В старых заголовках MinGW этой константы нет
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <cstdlib>
#include <unistd.h>
#endif

namespace
{
    /// Консоль умеет показывать ANSI-цвета (выясняется в initialize).
    bool g_colorSupported = false;
    /// Цвета сейчас включены.
    bool g_colorEnabled = false;
#ifdef _WIN32
    /// Исходный буфер std::cin, запомненный при запуске программы.
    /// В консоли Windows читаем через ReadConsoleW, а не через std::cin.
    /// Но если буфер std::cin подменили (так делают тесты), читаем из std::cin,
    /// иначе тест, запущенный из консоли, будет ждать ввода с клавиатуры.
    const std::streambuf* const g_originalCinBuffer = std::cin.rdbuf();
#endif

    /// Сколько пустых строк печатать вместо очистки экрана, если цветов нет.
    constexpr std::size_t FALLBACK_CLEAR_LINES = 40;

    /// Возвращает ANSI-код цвета.
    /// Коды: красный 31, зелёный 32, жёлтый 33, синий 34, пурпурный 35,
    /// голубой 36, серый 90, сброс 0.
    const char* getColorCode(ConsoleColor color)
    {
        switch (color)
        {
            case ConsoleColor::Red:     return "\x1b[31m";
            case ConsoleColor::Green:   return "\x1b[32m";
            case ConsoleColor::Yellow:  return "\x1b[33m";
            case ConsoleColor::Blue:    return "\x1b[34m";
            case ConsoleColor::Magenta: return "\x1b[35m";
            case ConsoleColor::Cyan:    return "\x1b[36m";
            case ConsoleColor::Gray:    return "\x1b[90m";
            case ConsoleColor::Default: break;
        }
        return "\x1b[0m";
    }

    /// Читает строку побайтно из std::cin.
    /// Используется на Mac и на Windows, когда ввод перенаправлен из файла или подменён в тестах.
    ReadStatus readStreamLine(char* buffer, std::size_t capacity)
    {
        std::size_t length = 0;
        bool tooLong = false;
        bool gotAnyChar = false;
        char ch = '\0';

        while (std::cin.get(ch))
        {
            gotAnyChar = true;
            if (ch == '\n')
            {
                break;
            }
            if (ch == '\r')
            {
                continue;   // файл с окончаниями строк Windows
            }
            if (length + 1 < capacity)
            {
                buffer[length] = ch;
                ++length;
            }
            else
            {
                tooLong = true;   // дочитываем строку до конца, но не сохраняем
            }
        }

        if (!gotAnyChar || tooLong)
        {
            buffer[0] = '\0';
            return gotAnyChar ? ReadStatus::TooLong : ReadStatus::EndOfInput;
        }
        buffer[length] = '\0';
        return ReadStatus::Ok;
    }

#ifdef _WIN32
    /// Максимальная длина строки в символах UTF-16 при чтении из консоли Windows.
    constexpr std::size_t WIDE_LINE_CAPACITY = 1024;
    /// Код символа Ctrl+Z: в консоли Windows он означает конец ввода.
    constexpr wchar_t CTRL_Z = 0x1A;

    bool hasEnvironmentVariable(const char* name)
    {
        // Возвращает 0, если переменной нет (getenv в MSVC даёт предупреждение)
        return GetEnvironmentVariableA(name, nullptr, 0) > 0;
    }

    bool detectColorSupport()
    {
        if (hasEnvironmentVariable("NO_COLOR"))
        {
            return false;
        }
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (output == INVALID_HANDLE_VALUE || GetConsoleMode(output, &mode) == 0)
        {
            return false;   // вывод перенаправлен в файл
        }
        if ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0)
        {
            return true;
        }
        // Windows 10+ понимает ANSI-коды, только если включить этот режим.
        // В старой консоли (Windows 7/8) вызов не сработает — тогда без цветов.
        return SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
    }

    void setupEncoding()
    {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }

    /// Читает строку из консоли Windows как UTF-16 и переводит в UTF-8.
    /// ReadFile/std::cin с кодовой страницей UTF-8 возвращают вместо
    /// русских букв нули, поэтому нужен ReadConsoleW.
    ReadStatus readConsoleLine(HANDLE input, char* buffer, std::size_t capacity)
    {
        wchar_t wideLine[WIDE_LINE_CAPACITY];
        std::size_t length = 0;
        bool tooLong = false;
        bool lineEnded = false;

        while (!lineEnded)
        {
            wchar_t chunk[256];
            DWORD readCount = 0;
            if (ReadConsoleW(input, chunk, 256, &readCount, nullptr) == 0 || readCount == 0)
            {
                buffer[0] = '\0';
                return ReadStatus::EndOfInput;
            }
            for (std::size_t i = 0; i < readCount; ++i)
            {
                if (chunk[i] == L'\n')
                {
                    lineEnded = true;
                    break;
                }
                if (chunk[i] == L'\r')
                {
                    continue;
                }
                if (length < WIDE_LINE_CAPACITY)
                {
                    wideLine[length] = chunk[i];
                    ++length;
                }
                else
                {
                    tooLong = true;
                }
            }
        }

        buffer[0] = '\0';
        if (length > 0 && wideLine[0] == CTRL_Z)
        {
            return ReadStatus::EndOfInput;
        }
        if (tooLong)
        {
            return ReadStatus::TooLong;
        }
        if (length == 0)
        {
            return ReadStatus::Ok;
        }

        const int wideLength = static_cast<int>(length);
        // Сначала узнаём, сколько байт займёт строка в UTF-8
        const int neededBytes = WideCharToMultiByte(CP_UTF8, 0, wideLine, wideLength,
                                                    nullptr, 0, nullptr, nullptr);
        if (neededBytes <= 0 || static_cast<std::size_t>(neededBytes) >= capacity)
        {
            return ReadStatus::TooLong;
        }
        WideCharToMultiByte(CP_UTF8, 0, wideLine, wideLength,
                            buffer, neededBytes, nullptr, nullptr);
        buffer[neededBytes] = '\0';
        return ReadStatus::Ok;
    }
#else
    bool detectColorSupport()
    {
        if (std::getenv("NO_COLOR") != nullptr)
        {
            return false;
        }
        if (isatty(STDOUT_FILENO) == 0)
        {
            return false;   // вывод в файл или в консоль IDE без эмуляции терминала
        }
        const char* term = std::getenv("TERM");
        return term != nullptr && std::string_view(term) != "dumb";
    }
#endif
}

namespace Console
{
    void initialize()
    {
#ifdef _WIN32
        setupEncoding();
#endif
        g_colorSupported = detectColorSupport();
        g_colorEnabled = g_colorSupported;
    }

    bool isColorEnabled()
    {
        return g_colorEnabled;
    }

    void setColorEnabled(bool enabled)
    {
        g_colorEnabled = enabled && g_colorSupported;
    }

    void setColor(ConsoleColor color)
    {
        if (g_colorEnabled)
        {
            std::cout << getColorCode(color);
        }
    }

    void resetColor()
    {
        setColor(ConsoleColor::Default);
    }

    void printColored(const char* utf8Text, ConsoleColor color)
    {
        setColor(color);
        std::cout << utf8Text;
        resetColor();
    }

    void clearScreen()
    {
        if (g_colorEnabled)
        {
            // Очистить экран и поставить курсор в левый верхний угол
            std::cout << "\x1b[2J\x1b[H";
            return;
        }
        for (std::size_t i = 0; i < FALLBACK_CLEAR_LINES; ++i)
        {
            std::cout << '\n';
        }
    }

    std::size_t getDisplayWidth(const char* utf8Text)
    {
        std::size_t width = 0;
        for (std::size_t i = 0; utf8Text[i] != '\0'; ++i)
        {
            // Байты-продолжения символа UTF-8 имеют вид 10xxxxxx — их не считаем
            const auto byte = static_cast<unsigned char>(utf8Text[i]);
            if ((byte & 0xC0u) != 0x80u)
            {
                ++width;
            }
        }
        return width;
    }

    void printPadded(const char* utf8Text, std::size_t width)
    {
        std::cout << utf8Text;
        for (std::size_t i = getDisplayWidth(utf8Text); i < width; ++i)
        {
            std::cout << ' ';
        }
    }

    ReadStatus readLine(char* buffer, std::size_t capacity)
    {
        // Подсказка должна появиться на экране до того, как программа начнёт ждать ввод
        std::cout.flush();

        if (capacity == 0)
        {
            return ReadStatus::TooLong;
        }
#ifdef _WIN32
        HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode = 0;
        const bool isRealConsole = input != INVALID_HANDLE_VALUE
                                   && GetConsoleMode(input, &mode) != 0
                                   && std::cin.rdbuf() == g_originalCinBuffer;
        if (isRealConsole)
        {
            return readConsoleLine(input, buffer, capacity);
        }
#endif
        return readStreamLine(buffer, capacity);
    }
}
