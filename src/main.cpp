#include "tox.h"

#include <CLI/CLI.hpp>

/**
 * Main entry point.
 */
int main(int argc, char* argv[]) {
    CLI::App app{"Tox - A typed Lox implementation"};

    std::string script;
    app.add_option("script", script, "Script file to run")->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    Tox tox;
    if (!script.empty()) {
        tox.runFile(script);
    } else {
        tox.repl();
    }

    return 0;
}
