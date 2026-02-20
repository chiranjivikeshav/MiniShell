#include "shell.h"
#include <iostream>
#include <string>

void print_version() {
    std::cout << "MiniShell " << MINISHELL_VERSION << "\n";
}

int main(int argc, char* argv[]) {

    if (argc > 1) {
        std::string arg = argv[1];

        if (arg == "-v" || arg == "--version") {
            print_version();
            return 0;
        }

        std::cerr << "Unknown option: " << arg << "\n";
        return 1;
    }

    Shell shell;
    shell.run();
    return 0;
}
