#ifndef HISTORY_H
#define HISTORY_H

#include <climits>
#include <vector>
#include <string>

class History {
    static std::vector<std::string> commands;
    static std::string history_file;
    static constexpr int MAX_HISTORY_SHOW = 1000;
    static int up_down_index;
public:
    History();

    void loadHistory();
    static void saveHistory();
    static void add(const std::string& cmd);
    static std::string getBack();
    static void show(int n = INT_MAX);
    static std::string get(int index);
    static std::string previousHistory();
    static std::string nextHistory();
    static void resetUpDownIndex();
};

#endif