#include "signal_handler.h"
#include <signal.h>
#include <unistd.h>

void SignalHandler :: setupShellSignals()
{
    signal(SIGINT, shellSigintHandler);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
}

void SignalHandler :: setupChildSignals()
{
    signal(SIGINT,SIG_DFL);
    signal(SIGTSTP,SIG_DFL);
    signal(SIGQUIT,SIG_DFL);
}

void SignalHandler ::shellSigintHandler(int) {
    write(STDOUT_FILENO, "\n", 1);
}