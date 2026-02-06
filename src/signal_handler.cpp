#include "signal_handler.h"
#include <signal.h>

void SignalHandler :: setupShellSignals()
{
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
}

void SignalHandler :: setupChildSignals()
{
    signal(SIGINT,SIG_DFL);
    signal(SIGTSTP,SIG_DFL);
    signal(SIGQUIT,SIG_DFL);
}