#include "line_editor.h"
#include "history.h"
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>

termios LineEditor::orig_termios;

void LineEditor::enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void LineEditor::disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

std::string LineEditor::readLine()
{
    std::string buffer;
    size_t cursor = 0;

    printPrompt();

    while (true)
    {
        char c;
        read(STDIN_FILENO, &c, 1);

        // ENTER
        if (c == '\n')
        {
            std::cout << "\n";
            break;
        }

        // BACKSPACE
        else if (c == 127)
        {
            backSpace(buffer, cursor);
        }

        // CTRL+A
        else if (c == 1)
        {
            startOfLine(buffer, cursor);
        }

        // CTRL+E
        else if (c == 5)
        {
            endOfLine(buffer, cursor);
        }

        // CTRL+L
        else if (c == 12)
        {
            clearScreen(buffer, cursor);
        }

        // TAB
        else if (c == '\t')
        {
            autoCompletePlaceHolder(buffer, cursor);
        }

        // ESCAPE SEQUENCE (arrow keys)
        else if (c == '\x1b')
        {
            escapeSequence(buffer, cursor);
        }

        // NORMAL CHARACTER
        else
        {
            normalCharacter(buffer, cursor, c);
        }
    }
    return buffer;
}

void LineEditor::backSpace(std::string &buffer, size_t &cursor)
{
    if (cursor > 0)
    {
        buffer.erase(cursor - 1, 1);
        cursor--;
        refreshLine(buffer, cursor);
    }
}

void LineEditor::startOfLine(std::string &buffer, size_t &cursor)
{
    cursor = 0;
    refreshLine(buffer, cursor);
}

void LineEditor::endOfLine(std::string &buffer, size_t &cursor)
{
    cursor = buffer.size();
    refreshLine(buffer, cursor);
}

void LineEditor::clearScreen(const std::string &buffer, const size_t &cursor)
{
    // clear screen
    std::cout << "\033[2J";
    // move cursor to top
    std::cout << "\033[H";
    refreshLine(buffer, cursor);
}

void LineEditor::autoCompletePlaceHolder(std::string &buffer, size_t &cursor)
{
    // Extract the current token
    std::string token;
    size_t start = buffer.rfind(' ', cursor);

    std::vector<std::string> matches;

    if (start == std::string::npos)
    {
        token = buffer.substr(0, cursor);
        matches = getCommandMatch(token);
    }
    else
    {
        token = buffer.substr(start + 1, cursor - start - 1);
        matches = getDirectoryMatch(token);
    }

    if (matches.empty())
    {
        std::cout << "\a";
    }
    else if (matches.size() == 1)
    {
        std::string completion = matches[0].substr(token.size());

        buffer.insert(cursor, completion);
        cursor += completion.size();

        refreshLine(buffer, cursor);
    }
    else
    {
        std::cout << "\n";

        size_t count = std::min(matches.size(), MAX_VIEW);

        for (size_t i = 0; i < count; ++i)
            std::cout << matches[i] << "  ";

        if (matches.size() > MAX_VIEW)
            std::cout << "\n... (" << matches.size() - MAX_VIEW << " more)";

        std::cout << "\n";

        refreshLine(buffer, cursor);
    }
}

std::vector<std::string> LineEditor:: getDirectoryMatch(const std::string &token)
{
    if (token.empty()) return {};

    DIR* dir = opendir(".");
    struct dirent* entry;

    std::vector<std::string> matches;

    while ((entry = readdir(dir)) != nullptr)
    {
        std::string name = entry->d_name;

        if (name.rfind(token, 0) == 0)
            matches.push_back(name);
    }
    closedir(dir);
    return matches;
}

std::vector<std::string> LineEditor::getCommandMatch(const std::string &token)
{
    if (token.empty()) return {};

    std::vector<std::string> matches;

    const char* pathEnv = getenv("PATH");
    if (!pathEnv)
        return matches;

    std::stringstream ss(pathEnv);
    std::string dir;

    while (std::getline(ss, dir, ':'))
    {
        DIR* d = opendir(dir.c_str());
        if (!d)
            continue;

        struct dirent* entry;

        while ((entry = readdir(d)) != nullptr)
        {
            std::string name = entry->d_name;

            if (name.rfind(token, 0) == 0)
                matches.push_back(name);
        }

        closedir(d);
    }

    // remove duplicates
    std::sort(matches.begin(), matches.end());
    matches.erase(std::unique(matches.begin(), matches.end()), matches.end());

    return matches;
}

void LineEditor::escapeSequence(std::string &buffer, size_t &cursor)
{
    char seq[2];
    read(STDIN_FILENO, &seq[0], 1);
    read(STDIN_FILENO, &seq[1], 1);

    if (seq[0] == '[')
    {
        switch (seq[1])
        {
            // UP
        case 'A':
            buffer = History::previousHistory();
            cursor = buffer.size();
            refreshLine(buffer, cursor);
            break;

            // DOWN
        case 'B':
            buffer = History::nextHistory();
            cursor = buffer.size();
            refreshLine(buffer, cursor);
            break;

            // RIGHT
        case 'C':
            if (cursor < buffer.size())
            {
                cursor++;
                refreshLine(buffer, cursor);
            }
            break;

            // LEFT
        case 'D':
            if (cursor > 0)
            {
                cursor--;
                refreshLine(buffer, cursor);
            }
            break;
        }
    }
}

void LineEditor::normalCharacter(std::string &buffer, size_t &cursor, char &c)
{
    buffer.insert(cursor, 1, c);
    cursor++;
    refreshLine(buffer, cursor);
}

void LineEditor::refreshLine(const std::string &buffer, const size_t &cursor)
{
    std::cout << "\33[2K\r";      // clear line
    printPrompt();                // print prompt
    std::cout << buffer;          // print command buffer

    size_t end = buffer.size();

    // move cursor left
    if (size_t moveLeft = end - cursor; moveLeft > 0)
        std::cout << "\033[" << moveLeft << "D";
    std::cout.flush();
}

void LineEditor::printPrompt()
{
    char cwd[PATH_MAX];
    const char* home = getenv("HOME");

    std::cout << "\033[1;32mmnsh\033[0m:";

    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::string path(cwd);
        if (home && path.find(home) == 0)
            path.replace(0, std::string(home).length(), "~");

        std::cout << "\033[1;34m" << path << "\033[0m";
    } else {
        std::cout << "?";
    }

    std::cout << "$ ";
    std::cout.flush();
}