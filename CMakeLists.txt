cmake_minimum_required(VERSION 3.16)

project(Yatzy LANGUAGES CXX)

# Стандарт C++17 для всех компиляторов (MSVC по умолчанию собирает в C++14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# В CI предупреждения считаются ошибками, локально — нет
option(YATZY_WARNINGS_AS_ERRORS "Считать предупреждения ошибками" OFF)

# Общий набор флагов предупреждений и кодировки
function(yatzy_set_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /utf-8 /permissive-)
        if(YATZY_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
        if(YATZY_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

# ============================================================
# ШАБЛОН. Раскомментируйте блок, когда появятся его файлы.
# В add_library перечисляйте только .cpp, которые уже есть в репозитории.
# ============================================================

# --- Логика игры (всё, кроме main.cpp и интерфейса) ---
add_library(yatzy_core STATIC
        src/DiceSet.cpp
        src/Category.cpp
        src/ScoreCalculator.cpp
        src/ScoreCard.cpp
        src/Player.cpp
        src/Text.cpp
        # ... новые .cpp добавлять сюда
)
target_include_directories(yatzy_core PUBLIC include)
# Включить, когда влит PR Пилипенко Дмитрия с исправлением предупреждений:
# yatzy_set_warnings(yatzy_core)

# --- Интерфейс (Новикова Полина): отдельная библиотека ---
# Собирается и проверяется независимо от yatzy_core: если сборка или тест упали,
# по имени цели сразу видно, чья это часть — yatzy_ui или yatzy_core.
# Когда появятся ConsoleRenderer и Menu (им нужен Player), добавить:
# target_link_libraries(yatzy_ui PUBLIC yatzy_core)
add_library(yatzy_ui STATIC
        src/Console.cpp
        src/InputReader.cpp
)
target_include_directories(yatzy_ui PUBLIC include)
yatzy_set_warnings(yatzy_ui)

# --- Игра ---
# add_executable(yatzy src/main.cpp)
# target_link_libraries(yatzy PRIVATE yatzy_core yatzy_ui)
# yatzy_set_warnings(yatzy)

# --- Тесты: на каждый файл тестов — три строки + add_test ---
enable_testing()

add_executable(yatzy_tests tests/ScoreCalculatorTests.cpp)
target_link_libraries(yatzy_tests PRIVATE yatzy_core)
yatzy_set_warnings(yatzy_tests)
add_test(NAME score_calculator COMMAND yatzy_tests)

add_executable(yatzy_score_card_tests tests/ScoreCardTests.cpp)
target_link_libraries(yatzy_score_card_tests PRIVATE yatzy_core)
yatzy_set_warnings(yatzy_score_card_tests)
add_test(NAME score_card COMMAND yatzy_score_card_tests)

# --- Тесты интерфейса (Новикова Полина) ---

add_executable(console_tests tests/ConsoleTests.cpp)
target_link_libraries(console_tests PRIVATE yatzy_ui)
yatzy_set_warnings(console_tests)
add_test(NAME console COMMAND console_tests)

add_executable(input_reader_tests tests/InputReaderTests.cpp)
target_link_libraries(input_reader_tests PRIVATE yatzy_ui)
yatzy_set_warnings(input_reader_tests)
add_test(NAME input_reader COMMAND input_reader_tests)

add_executable(text_tests tests/TextTests.cpp)
target_link_libraries(text_tests PRIVATE yatzy_core)
yatzy_set_warnings(text_tests)
add_test(NAME text COMMAND text_tests)

# --- Ручная проверка консоли: цвета, выравнивание, ввод кириллицы ---
add_executable(console_check tools/ConsoleCheck.cpp)
target_link_libraries(console_check PRIVATE yatzy_ui)
yatzy_set_warnings(console_check)
