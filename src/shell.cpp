#include "shell.h"
#include "parser.h"
#include "token.h"
#include "ASTNode.h"
#include "AST_executor.h"
#include "signal_handler.h"
#include "history.h"


#include<iostream>
#include<string>
#include<memory>
#include <unistd.h>    // getcwd
#include <limits.h>    // PATH_MAX

void Shell::printPrompt()
{
    char cwd[PATH_MAX];
    const char* home = getenv("HOME");

    std::cout << "\033[1;32mmnsh\033[0m:";

    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::string path(cwd);
        if (home && path.find(home) == 0)
            path.replace(0, std::string(home).length(), "~");

        std::cout << "\033[1;34m" << path << "\033[0m";
    } else {
        std::cout << "?";
    }

    std::cout << "$ ";
    std::cout.flush();
}

void Shell::run()
{
    SignalHandler::setupShellSignals();

    std::string command;
    History history;
    Tokenizer tokenizer;
    ASTExecutor executor;
    while (true)
    {
        printPrompt();

        if (!std::getline(std::cin, command))
        {
            break;
        }else if (command.empty())
        {
            continue;
        }
        history.add(command);

        std::vector<Token> tokens = tokenizer.tokenize(command);
        Parser parser(tokens);
        std::unique_ptr<ASTNode> root = parser.parse();
        executor.execute(root.get());
    }
}
