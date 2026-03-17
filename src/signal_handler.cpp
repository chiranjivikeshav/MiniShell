#include "signal_handler.h"
#include <signal.h>
#include <unistd.h>
#include <history.h>
#include <line_editor.h>

void SignalHandler :: setupShellSignals()
{
    signal(SIGINT, shellSigintHandler);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
    signal(SIGHUP, shellExitHandler);
    signal(SIGTERM, shellExitHandler);
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

void SignalHandler::shellExitHandler(int)
{
    History::saveHistory();
    LineEditor::disableRawMode();

    _exit(0);
}