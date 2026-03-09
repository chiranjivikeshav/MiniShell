#ifndef HISTORY_H
#define HISTORY_H

#include <climits>
#include <vector>
#include <string>

class History {
    static std::vector<std::string> commands;
    static std::string history_file;
    const size_t MAX_HISTORY = 1000;
public:
    History();

    void loadHistory();
    static void saveHistory();
    void add(const std::string& cmd);
    static void show(int n = INT_MAX);
    static std::string get(int index);
};

#endif