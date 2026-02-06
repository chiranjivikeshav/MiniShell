#ifndef SIGNAL_H
#define SIGNAL_H

class SignalHandler
{
    public:
    static void setupShellSignals();
    static void setupChildSignals();
};

#endif