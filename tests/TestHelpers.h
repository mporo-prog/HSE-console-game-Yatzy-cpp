// TestHelpers.h — общие помощники для тестов интерфейса.
// Позволяют «напечатать» ввод заранее и спрятать вывод, чтобы проверять
// InputReader и Console без клавиатуры.
#pragma once

#include <cstddef>
#include <iostream>
#include <streambuf>

/// Источник ввода из готовой строки: std::cin будет читать из неё.
class TextInputBuffer : public std::streambuf
{
public:
    explicit TextInputBuffer(const char* text)
    {
        std::size_t length = 0;
        while (text[length] != '\0')
        {
            ++length;
        }
        char* begin = const_cast<char*>(text);
        setg(begin, begin, begin + length);
    }
};

/// Приёмник вывода, который всё выбрасывает.
class NullOutputBuffer : public std::streambuf
{
protected:
    int_type overflow(int_type ch) override { return traits_type::not_eof(ch); }
};

/// Пока объект жив, std::cin читает из text, а вывод std::cout не показывается.
class ScopedConsole
{
public:
    explicit ScopedConsole(const char* text)
        : m_input(text),
          m_oldInput(std::cin.rdbuf(&m_input)),
          m_oldOutput(std::cout.rdbuf(&m_output))
    {
        std::cin.clear();
    }

    ~ScopedConsole()
    {
        std::cin.rdbuf(m_oldInput);
        std::cout.rdbuf(m_oldOutput);
        std::cin.clear();
    }

    ScopedConsole(const ScopedConsole&) = delete;
    ScopedConsole& operator=(const ScopedConsole&) = delete;

private:
    TextInputBuffer m_input;
    NullOutputBuffer m_output;
    std::streambuf* m_oldInput;
    std::streambuf* m_oldOutput;
};

/// Счётчик проверок. В конце main: return reportResults();
inline int g_failedChecks = 0;

inline void check(bool condition, const char* description)
{
    if (!condition)
    {
        ++g_failedChecks;
        std::cerr << "ПРОВАЛ: " << description << '\n';
    }
}

inline int reportResults(const char* suiteName)
{
    if (g_failedChecks == 0)
    {
        std::cout << suiteName << ": все проверки прошли\n";
        return 0;
    }
    std::cerr << suiteName << ": провалено проверок — " << g_failedChecks << '\n';
    return 1;
}
