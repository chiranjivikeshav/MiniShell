#include "executor.h"
#include "history.h"
#include "AST_executor.h"
#include "parser.h"
#include "token.h"

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <fcntl.h>
#include <signal_handler.h>

int Executor::execute(const Command& cmd)
{
    if (cmd.name.empty())
    {
        return 0;
    }
    if (cmd.name == "exit")
    {
        History::saveHistory();
        exit(0);
    }
    if (cmd.name == "cd")
    {
        return handleCD(cmd);
    }
    if (cmd.name == "history")
    {
        return handleHistory(cmd);
    }
    if (!cmd.name.empty() && cmd.name[0] == '!')
    {
        return handleHistoryExpansion(cmd);
    }
    return handleChildProcess(cmd);
}

int Executor::handleChildProcess(const Command& cmd)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        SignalHandler::setupChildSignals();

        std::vector<char*> argv;
        for (const auto& arg : cmd.args)
            argv.push_back(const_cast<char*>(arg.c_str()));
        argv.push_back(nullptr);

        handleRedirection(cmd.redirections);
        execvp(cmd.name.c_str(), argv.data());

        perror("execution failed");
        _exit(127);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 0;
}

int Executor::handleCD(const Command& cmd)
{
    std::string targetDir;
    if (cmd.args.size() == 1 || cmd.args[1] == "~")
    {
        targetDir = getenv("HOME");
    }else
    {
        targetDir = cmd.args[1];
    }
    if (chdir(targetDir.c_str()) < 0)
    {
        perror("command failed");
        return 1;
    }
    return 0;
}

void Executor::handleRedirection(const std::vector<Redirection>& rds)
{
    for (auto& r : rds) {
        switch (r.type) {
        case RedirectType::INPUT:
            redirect(0, r.file, O_RDONLY);
            break;
        case RedirectType::OUTPUT:
            redirect(1, r.file, O_WRONLY|O_CREAT|O_TRUNC);
            break;
        case RedirectType::APPEND:
            redirect(1, r.file, O_WRONLY|O_CREAT|O_APPEND);
            break;
        case RedirectType::STDERR:
            redirect(2, r.file, O_WRONLY|O_CREAT|O_TRUNC);
            break;
        }
    }
}

void Executor::redirect(const int targetFd, const std::string& file, const int flags)
{
    int fd = open(file.c_str(), flags, 0644);
    if (fd < 0) {
        perror("redirection failed");
        _exit(1);
    }

    if (dup2(fd, targetFd) < 0) {
        perror("dup2 failed");
        _exit(1);
    }

    close(fd);
}

int Executor::handleHistory(const Command& cmd)
{
    if (cmd.args.size() == 1)
    {
        History::show();
        return 0;
    }
    try
    {
        int n = std::stoi(cmd.args[1]);
        if (n < 0)
        {
            std::cerr << "history: invalid argument\n";
            return 1;
        }
        History::show(n);
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "history: numeric argument required" << std::endl;
    }
    return 1;
}

int Executor::handleHistoryExpansion(const Command& cmd)
{
    std::string command;

    if (cmd.name == "!!")
    {
        command = History::getBack();
        if (command.empty())
        {
            std::cerr << "mnsh: !!: event not found\n";
            return 1;
        }
    }
    else
    {
        try
        {
            int id = std::stoi(cmd.name.substr(1));
            command = History::get(id-1);
            if (command.empty())
            {
                std::cerr << "mnsh: !!: event not found\n";
                return 1;
            }
        }
        catch ( const std::exception& e )
        {
            std::cerr << "mnsh: " << cmd.name << ": event not found\n";
            return 1;
        }
    }

    // print expanded command
    std::cout << command << std::endl;

    // add executed command back to history
    History::add(command);

    // execute command
    Tokenizer tokenizer;
    ASTExecutor executor;
    std::vector<Token> tokens = tokenizer.tokenize(command);
    Parser parser(tokens);
    std::unique_ptr<ASTNode> root = parser.parse();
    return executor.execute(root.get());
}