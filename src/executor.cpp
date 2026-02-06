#include "executor.h"

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <fcntl.h>
#include <signal_handler.h>

int Executor::execute(const Command& cmd)
{
    if (cmd.name == "exit")
    {
        exit(0);
    }
    if (cmd.name == "cd")
    {
        return handleCD(cmd);
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
    int status;
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
