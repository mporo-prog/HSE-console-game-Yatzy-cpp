// ConsoleCheck.cpp — ручная проверка консоли: кодировка, цвета, ввод кириллицы.
// Ответственный: Новикова Полина (Интерфейс).
#include <cstddef>
#include <iomanip>
#include <iostream>

#include "Console.h"
#include "InputReader.h"

namespace
{
    /// Цвет и его название для меню выбора.
    struct NamedColor
    {
        const char* name;
        ConsoleColor color;
    };

    constexpr NamedColor COLORS[] = {
        {"красный", ConsoleColor::Red},
        {"зелёный", ConsoleColor::Green},
        {"жёлтый", ConsoleColor::Yellow},
        {"синий", ConsoleColor::Blue},
        {"пурпурный", ConsoleColor::Magenta},
        {"голубой", ConsoleColor::Cyan},
        {"серый", ConsoleColor::Gray},
        {"обычный", ConsoleColor::Default},
    };
    constexpr int COLOR_COUNT = static_cast<int>(sizeof(COLORS) / sizeof(COLORS[0]));

    /// Текст для проверки цвета, если пользователь ничего не ввёл.
    constexpr const char* SAMPLE_TEXT = "Яцзы: Каре, Фул-хаус, Большой стрит — 40 очков";

    void showColorStatus()
    {
        std::cout << "Цвета: " << (Console::isColorEnabled() ? "включены" : "выключены") << '\n';
    }

    void showAllColors()
    {
        showColorStatus();
        for (const NamedColor& item : COLORS)
        {
            Console::printColored(item.name, item.color);
            std::cout << ' ';
        }
        std::cout << "\n\n";
    }

    void showAlignment()
    {
        std::cout << "Выравнивание:\n";
        const char* rows[] = {"Единицы", "Большой стрит", "Яцзы", "Ёлка"};
        for (const char* row : rows)
        {
            std::cout << "│ ";
            Console::printPadded(row, 14);
            std::cout << "│\n";
        }
        std::cout << '\n';
    }

    /// Печатает введённую строку и её байты, чтобы было видно, не испортилась ли кодировка.
    void showInput(const char* text)
    {
        std::cout << "Вы ввели: «" << text << "»\n";
        std::cout << "Символов: " << Console::getDisplayWidth(text) << ", байты:";
        for (std::size_t i = 0; text[i] != '\0'; ++i)
        {
            std::cout << ' ' << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                      << static_cast<unsigned>(static_cast<unsigned char>(text[i]));
        }
        std::cout << std::dec << std::setfill(' ') << '\n';
        std::cout << '\n';
    }

    /// Проверка ввода: текст по-русски и число с проверкой диапазона.
    void checkInput(InputReader& reader)
    {
        char buffer[INPUT_BUFFER_SIZE];
        reader.readLine("Введите текст по-русски: ", buffer, INPUT_BUFFER_SIZE);
        showInput(buffer);
        const int number = reader.readInt("Введите число от 1 до 6: ", 1, 6);
        std::cout << "Принято число " << number << "\n\n";
    }

    /// Пользователь выбирает цвет и видит в нём свой текст или пример.
    void checkChosenColor(InputReader& reader)
    {
        for (int i = 0; i < COLOR_COUNT; ++i)
        {
            std::cout << "  " << (i + 1) << " — ";
            Console::printColored(COLORS[i].name, COLORS[i].color);
            std::cout << '\n';
        }
        const int choice = reader.readInt("Номер цвета: ", 1, COLOR_COUNT);
        const NamedColor& chosen = COLORS[choice - 1];

        char text[INPUT_BUFFER_SIZE];
        reader.readLine("Текст для проверки (Enter — пример): ", text, INPUT_BUFFER_SIZE);
        const char* shown = (text[0] == '\0') ? SAMPLE_TEXT : text;

        std::cout << "Цвет «" << chosen.name << "»: ";
        Console::printColored(shown, chosen.color);
        std::cout << '\n';
        if (!Console::isColorEnabled())
        {
            std::cout << "Цвета выключены, поэтому текст показан обычным цветом.\n";
        }
        std::cout << '\n';
    }

    /// Включает или выключает цвета (включить можно, только если консоль их поддерживает).
    void toggleColors()
    {
        const bool wanted = !Console::isColorEnabled();
        Console::setColorEnabled(wanted);
        if (wanted && !Console::isColorEnabled())
        {
            std::cout << "Эта консоль не поддерживает цвета (или вывод не в консоль).\n";
        }
        showAllColors();
    }
}

int main()
{
    Console::initialize();
    showAllColors();
    showAlignment();

    InputReader reader;
    while (true)
    {
        try
        {
            std::cout << "1 — проверить ввод\n"
                         "2 — выбрать цвет\n"
                         "3 — включить или выключить цвета\n"
                         "«меню» — проверить команду меню, «выход» — закончить\n";
            const int action = reader.readInt("Действие: ", 1, 3);
            std::cout << '\n';
            switch (action)
            {
                case 1: checkInput(reader); break;
                case 2: checkChosenColor(reader); break;
                default: toggleColors(); break;
            }
        }
        catch (const MenuRequested&)
        {
            Console::printColored("Команда «меню» распознана.\n\n", ConsoleColor::Green);
        }
        catch (const ExitRequested&)
        {
            Console::printColored("Команда «выход» распознана.\n", ConsoleColor::Green);
            return 0;
        }
    }
}
