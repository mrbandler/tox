#include "tox.h"

#include "scanner.h"

#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <stdexcept>
#include <string>

void Tox::runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    auto src = buffer.str();

    run(src);

    if (m_hadError) {
        std::exit(65);
    }
}

void Tox::run(const std::string& src) {
    // Scan the source code into tokens.
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();

    for (const auto& token : tokens) {
        std::println("{}", token.toString());
    }
}

void Tox::repl() {
    std::println("Tox REPL");
    std::println("Type 'exit' or press Ctrl+C to quit.");

    std::string line;
    while (true) {
        std::print("> ");

        if (!std::getline(std::cin, line)) {
            std::print("\n");

            break;
        }

        if (line == "exit" || line == "quit") {
            break;
        }

        if (line.empty()) {
            continue;
        }

        run(line);

        m_hadError = false;
    }
}

void Tox::error(std::size_t line, std::string_view msg) {
    report(line, "", msg);
}

void Tox::report(std::size_t line, std::string_view where, std::string_view msg) {
    std::println(stderr, "[line {}] Error{}: {}", line, where, msg);
}
