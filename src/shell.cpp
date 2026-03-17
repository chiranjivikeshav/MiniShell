#include "shell.h"
#include "parser.h"
#include "token.h"
#include "ASTNode.h"
#include "AST_executor.h"
#include "signal_handler.h"
#include "history.h"
#include "line_editor.h"


#include<iostream>
#include<string>
#include<memory>
#include <unistd.h>    // getcwd
#include <limits.h>    // PATH_MAX

void Shell::run()
{
    SignalHandler::setupShellSignals();
    atexit(History::saveHistory);

    std::string command;
    History history;
    Tokenizer tokenizer;
    ASTExecutor executor;
    while (true)
    {
        command = LineEditor::readLine();

        if (command.empty())
        {
            continue;
        }
        if (command.find('!') == std::string::npos) history.add(command);

        std::vector<Token> tokens = tokenizer.tokenize(command);
        Parser parser(tokens);
        std::unique_ptr<ASTNode> root = parser.parse();
        executor.execute(root.get());

        History::resetUpDownIndex();
    }
}
