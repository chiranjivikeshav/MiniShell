#ifndef EXECUTER_H
#define EXECUTER_H

#include "command.h"
#include "redirection.h"

class Executor
{
public:
    int execute(const Command& cmd);

private:
    int handleCD(const Command& cmd);
    void handleRedirection(const std::vector<Redirection>& rds);
    void redirect(int targetFd, const std::string& file, int flags);
    int handleChildProcess(const Command& cmd);
    int handleHistory(const Command& cmd);
    int handleHistoryExpansion(const Command& cmd);
};

#endif