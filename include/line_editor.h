#ifndef LINE_EDITOR_H
#define LINE_EDITOR_H

#include <string>

class LineEditor
{
    static struct termios orig_termios;
    static void backSpace(std::string &buffer, size_t &cursor);
    static void startOfLine(std::string &buffer, size_t &cursor);
    static void endOfLine(std::string &buffer, size_t &cursor);
    static void clearScreen(const std::string &buffer,const size_t &cursor);
    static void autoCompletePlaceHolder();
    static void escapeSequence(std::string &buffer, size_t &cursor);
    static void normalCharacter(std::string &buffer, size_t &cursor, char &c);
    static void refreshLine(const std::string &buffer, const size_t &cursor);
    static void printPrompt();
public:
    static std::string readLine();
    static void enableRawMode();
    static void disableRawMode();
};

#endif