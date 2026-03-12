#include "shell.h"
#include "line_editor.h"
#include <iostream>
#include <string>

void print_version() {
    std::cout << "MiniShell " << MINISHELL_VERSION << "\n";
}

void updateTitle() {
    std::cout << "\033]0;MiniShell\007";
}

int main(int argc, char* argv[]) {
    updateTitle();

    // raw terminal mode enable and disable on exit
    LineEditor::enableRawMode();
    atexit(LineEditor::disableRawMode);

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
