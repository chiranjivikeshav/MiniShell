#ifndef LINE_EDITOR_H
#define LINE_EDITOR_H

#include <string>

class LineEditor
{
    static struct termios orig_termios;
public:
    static std::string readLine();
    static void enableRawMode();
    static void disableRawMode();
};

#endif