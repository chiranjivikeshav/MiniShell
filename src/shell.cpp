#include "shell.h"
#include "parser.h"
#include "token.h"
#include "ASTNode.h"
#include "AST_executor.h"
#include "signal_handler.h"


#include<iostream>
#include<string>
#include<memory>

void Shell::printPrompt()
{
    std::cout<<"$ ";
    std::cout.flush();
}

void Shell::run()
{
    SignalHandler::setupShellSignals();

    std::string command;
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

        std::vector<Token> tokens = tokenizer.tokenize(command);
        Parser parser(tokens);
        std::unique_ptr<ASTNode> root = parser.parse();
        executor.execute(root.get());
    }
}
