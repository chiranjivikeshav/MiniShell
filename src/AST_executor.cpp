#include "AST_executor.h"

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include "executor.h"
#include "subshellNode.h"
#include "signal_handler.h"

int ASTExecutor::execute(const ASTNode* node)
{
    if (!node) return 0;

    switch (node->type)
    {
    case NodeType::COMMAND:
        {
            auto* cmd = static_cast<const Command*>(node);
            Executor exec;
            return exec.execute(*cmd);
        }

    case NodeType::SEQUENCE:
        {
            execute(node->left.get());
            return execute(node->right.get());
        }

    case NodeType::AND:
        {
            int status = execute(node->left.get());
            if (status == 0)
                return execute(node->right.get());
            return status;
        }

    case NodeType::OR:
        {
            int status = execute(node->left.get());
            if (status != 0)
                return execute(node->right.get());
            return status;
        }

    case NodeType::PIPE:
        {
            int fd[2];
            if (pipe(fd) < 0) {
                perror("pipe");
                return 1;
            }

            pid_t leftPid = fork();
            if (leftPid == 0) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
                execute(node->left.get());
                _exit(0);
            }

            pid_t rightPid = fork();
            if (rightPid == 0) {
                dup2(fd[0], STDIN_FILENO);
                close(fd[1]);
                close(fd[0]);
                execute(node->right.get());
                _exit(0);
            }

            close(fd[0]);
            close(fd[1]);

            int status;
            waitpid(leftPid, &status, 0);
            waitpid(rightPid, &status, 0);
            return WEXITSTATUS(status);
        }

    case NodeType::SUBSHELL:
        {
            const auto sub = dynamic_cast<const SubshellNode*>(node);
            return executeSubshell(sub->child.get());
        }
    }
    return 0;
}

int ASTExecutor::executeSubshell(const ASTNode* subtree)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        SignalHandler::setupChildSignals();
        const int status = execute(subtree);
        _exit(status);
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1;
}