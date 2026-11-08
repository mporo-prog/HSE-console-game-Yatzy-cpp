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

# --- Логика игры (всё, кроме main.cpp) ---
 add_library(yatzy_core STATIC
         src/DiceSet.cpp
         src/Category.cpp
         src/ScoreCalculator.cpp
         src/ScoreCard.cpp
         src/Player.cpp
         # ... новые .cpp добавлять сюда
 )
# target_include_directories(yatzy_core PUBLIC include)
# yatzy_set_warnings(yatzy_core)

# --- Игра ---
# add_executable(yatzy src/main.cpp)
# target_link_libraries(yatzy PRIVATE yatzy_core)
# yatzy_set_warnings(yatzy)

# --- Тесты: на каждый файл тестов — три строки + add_test ---
# enable_testing()
# add_executable(yatzy_tests tests/ScoreCalculatorTests.cpp)
# target_link_libraries(yatzy_tests PRIVATE yatzy_core)
# yatzy_set_warnings(yatzy_tests)
# add_test(NAME score_calculator COMMAND yatzy_tests)