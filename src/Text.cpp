// Text.cpp — работа со строками UTF-8.
#include "Text.h"

namespace
{
    /// Возвращает true, если байт — продолжение буквы UTF-8 (вид 10xxxxxx).
    /// Первый байт буквы и обычные латинские символы так не выглядят.
    bool isContinuationByte(char byte)
    {
        return (static_cast<unsigned char>(byte) & 0xC0u) == 0x80u;
    }
}

namespace Text
{
    std::size_t length(const char* text)
    {
        if (text == nullptr)
        {
            return 0;
        }
        std::size_t count = 0;
        while (text[count] != '\0')
        {
            ++count;
        }
        return count;
    }

    bool copy(char* dest, std::size_t destSize, const char* src)
    {
        if (dest == nullptr || destSize == 0)
        {
            return false;
        }
        if (src == nullptr)
        {
            dest[0] = '\0';
            return true;
        }

        std::size_t count = 0;
        while (count + 1 < destSize && src[count] != '\0')
        {
            dest[count] = src[count];
            ++count;
        }

        const bool truncated = src[count] != '\0';
        if (truncated)
        {
            // Если обрезали посреди буквы, следующий байт в src — её продолжение.
            // Откатываемся назад, пока не уберём все уже скопированные байты этой буквы.
            while (count > 0 && isContinuationByte(src[count]))
            {
                --count;
            }
        }
        dest[count] = '\0';
        return !truncated;
    }
}
