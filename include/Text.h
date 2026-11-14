// Text.h — работа со строками UTF-8 без std::string и функций из C.
// Ответственный: Новикова Полина (Интерфейс).
// Модуль входит в yatzy_core: его используют Player и Game.
#pragma once

#include <cstddef>

namespace Text
{
    /// Возвращает длину строки в байтах без завершающего '\0'.
    /// Для nullptr возвращает 0. Замена strlen, которая запрещена в README.
    std::size_t length(const char* text);

    /// Копирует src в dest — массив на destSize байт. Строка в dest всегда завершается '\0'.
    /// Слишком длинная строка обрезается по границе буквы UTF-8: русская буква
    /// занимает 2 байта и не разрезается пополам.
    /// src == nullptr считается пустой строкой.
    /// Возвращает true, если строка скопирована целиком, и false, если её пришлось обрезать
    /// или dest == nullptr, destSize == 0.
    bool copy(char* dest, std::size_t destSize, const char* src);
}
