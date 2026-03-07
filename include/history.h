#ifndef HISTORY_H
#define HISTORY_H

#include <climits>
#include <vector>
#include <string>

class History {
    static std::vector<std::string> commands;

public:
    void add(const std::string& cmd);
    void show(int n = INT_MAX) const;
    std::string get(int index) const;
};

#endif