#include "history.h"

#include <climits>
#include <iostream>

std::vector<std::string>History::commands;

void History::add(const std::string& cmd)
{
    if (!cmd.empty())
        commands.push_back(cmd);
}

void History::show(int n) const
{
    int size = commands.size();

    if (n == INT_MAX || n > size)
        n = size;

    int start = size - n;

    for (int i = start; i < size; i++)
    {
        std::cout << i + 1 << "  " << commands[i] << std::endl;
    }
}

std::string History::get(int index) const
{
    if (index < 0 || index >= commands.size())
        return "";

    return commands[index];
}