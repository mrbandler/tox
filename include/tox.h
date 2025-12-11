#pragma once

#include "token.h"

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>

// Forward declarations.
class Interpreter;
class RuntimeError;

/**
 * Tox interpreter.
 */
class Tox {
private:
    /**
     * Flag, whether an error has occurred.
     */
    static bool hadError;

    /**
     * Flag, whether a runtime error has occurred.
     */
    static bool hadRuntimeError;

    /**
     * The interpreter instance.
     */
    std::unique_ptr<Interpreter> m_interpreter;

public:
    /**
     * Constructs a new Tox interpreter.
     */
    Tox();

    /**
     * Destructor.
     */
    ~Tox();

    /*
     * Runs the source code from a file.
     *
     * @param path Path to the source code file.
     * @return Exit code (0 for success, non-zero for errors).
     */
    int runFile(const std::string& path);

    /*
     * Runs the given source code.
     *
     * @param src Source code to run.
     */
    void run(const std::string& src);

    /**
     * Read-Eval-Print Loop (REPL) for Tox.
     *
     * @return Exit code (0 for success, non-zero for errors).
     */
    int repl();

    /**
     * Reports an error at a specific line.
     *
     * @param line Line number where the error occurred.
     * @param msg Error message.
     *
     */
    static void error(std::size_t line, std::string_view msg);

    /*
     * Reports an error at a specific token.
     *
     * @param token Token where the error occurred.
     * @param msg Error message.
     */
    static void error(const Token& token, std::string_view msg);

    /**
     * Reports a runtime error.
     *
     * @param error The runtime error to report.
     */
    static void runtimeError(const RuntimeError& error);

    /**
     * Reports an error at a specific line and location.
     *
     * @param line Line number where the error occurred.
     * @param where Location within the line.
     * @param msg Error message.
     */
    static void report(std::size_t line, std::string_view where, std::string_view msg);
};
