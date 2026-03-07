cmake_minimum_required(VERSION 3.16)

project(mnsh VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

add_executable(mnsh
        main.cpp
        src/shell.cpp
        src/executor.cpp
        src/parser.cpp
        src/token.cpp
        src/AST_executor.cpp
        src/signal_handler.cpp
        src/history.cpp
)

target_include_directories(mnsh PRIVATE include model)

target_compile_definitions(mnsh
        PRIVATE MINISHELL_VERSION="${PROJECT_VERSION}"
)