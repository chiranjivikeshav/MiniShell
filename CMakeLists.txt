cmake_minimum_required(VERSION 3.16)
project(minishell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include model)

add_executable(minishell
        main.cpp
        src/shell.cpp
        src/executor.cpp
        src/parser.cpp
        src/token.cpp
        src/AST_executor.cpp
        src/signal_handler.cpp
)

