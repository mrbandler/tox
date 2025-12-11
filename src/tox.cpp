#include "tox.h"

#include "interpreter.h"
#include "parser.h"
#include "scanner.h"

#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <stdexcept>
#include <string>

constexpr int EXIT_SUCCESS_CODE = 0;
constexpr int EXIT_SYNTAX_ERROR = 65;
constexpr int EXIT_RUNTIME_ERROR = 70;

bool Tox::hadError = false;
bool Tox::hadRuntimeError = false;

Tox::Tox() : m_interpreter(std::make_unique<Interpreter>()) {}

Tox::~Tox() = default;

int Tox::runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    auto src = buffer.str();

    run(src);

    if (hadError) {
        return EXIT_SYNTAX_ERROR;
    }

    if (hadRuntimeError) {
        return EXIT_RUNTIME_ERROR;
    }

    return EXIT_SUCCESS_CODE;
}

void Tox::run(const std::string& src) {
    // Scan the source code into tokens.
    Scanner scanner(src);
    auto tokens = scanner.scanTokens();

    Parser parser = Parser(tokens);
    auto expr = parser.parse();

    if (hadError) {
        return;
    }

    m_interpreter->interpret(*expr);
}

int Tox::repl() {
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

        hadError = false;
    }

    return EXIT_SUCCESS_CODE;
}

void Tox::error(std::size_t line, std::string_view msg) {
    report(line, "", msg);
}

void Tox::error(const Token& token, std::string_view msg) {
    if (token.type == TokenType::END_OF_FILE) {
        report(token.line, " at end", msg);
    } else {
        report(token.line, std::format(" at '{}'", token.lexeme), msg);
    }
}

void Tox::runtimeError(const RuntimeError& error) {
    std::println(stderr, "{}\n[line {}]", error.what(), error.token().line);

    hadRuntimeError = true;
}

void Tox::report(std::size_t line, std::string_view where, std::string_view msg) {
    std::println(stderr, "[line {}] Error{}: {}", line, where, msg);
}
