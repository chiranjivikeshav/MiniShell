cmake_minimum_required(VERSION 3.16)
project(minishell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy)
set(MINISHELL_VERSION "1.0")

include_directories(include model)
add_compile_definitions(MINISHELL_VERSION="${MINISHELL_VERSION}")

add_executable(minishell
        main.cpp
        src/shell.cpp
        src/executor.cpp
        src/parser.cpp
        src/token.cpp
        src/AST_executor.cpp
        src/signal_handler.cpp
)

