#pragma once

#include <cstddef>
#include <string>
#include <string_view>

/**
 * Tox interpreter.
 */
class Tox {
public:
    /*
     * Runs the source code from a file.
     *
     * @param path Path to the source code file.
     */
    void runFile(const std::string& path);

    /*
     * Runs the given source code.
     *
     * @param src Source code to run.
     */
    void run(const std::string& src);

    /**
     * Read-Eval-Print Loop (REPL) for Tox.
     */
    void repl();

    /**
     * Reports an error at a specific line.
     *
     * @param line Line number where the error occurred.
     * @param msg Error message.
     *
     */
    static void error(std::size_t line, std::string_view msg);

    /**
     * Reports an error at a specific line and location.
     *
     * @param line Line number where the error occurred.
     * @param where Location within the line.
     * @param msg Error message.
     */
    static void report(std::size_t line, std::string_view where, std::string_view msg);

private:
    /**
     * Flag, whether an error has occurred.
     */
    bool m_hadError = false;
};
