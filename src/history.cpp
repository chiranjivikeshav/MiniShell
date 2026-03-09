#include "history.h"

#include <climits>
#include <iostream>
#include <fstream>

std::vector<std::string>History::commands;
std::string History:: history_file;
History::History()
{
    const char* home = getenv("HOME");

    if (home)
        history_file = std::string(home) + "/.mnsh_history";
    else
        history_file = ".mnsh_history";
    loadHistory();
}

void History::add(const std::string& cmd)
{
    if (cmd.empty()) return;
    if (commands.empty() || commands.back() != cmd)
        commands.push_back(cmd);

    while(commands.size() > MAX_HISTORY)
        commands.erase(commands.begin());
}

void History::show(int n)
{
    int size = int(commands.size());
    if (n == INT_MAX || n > size)
        n = size;

    int start = size - n;

    for (int i = start; i < size; i++)
    {
        std::cout << i + 1 << "  " << commands[i] << std::endl;
    }
}

std::string History::get(int index)
{
    if (index < 0 || index >= commands.size())
        return "";

    return commands[index];
}

void History::loadHistory()
{
    std::ifstream in(history_file);

    if (!in.is_open()) {
        std::ofstream create(history_file);
        return;
    }
    std::string line;
    while (std::getline(in, line))
        commands.push_back(line);
}

void History::saveHistory()
{
    std::ofstream out(history_file);

    for (const auto& cmd : commands)
        out << cmd << "\n";
}