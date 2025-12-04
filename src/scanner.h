#pragma once

#include "token.h"

#include <unordered_map>
#include <vector>

/**
 * Scanner for source code.
 */
class Scanner {

public:
    /**
     * Default constructor.
     *
     * @param source Source code to scan.
     */
    Scanner(std::string source);

    /**
     * Scans the source code and returns a list of tokens.
     * k
     * @return List of scanned tokens.
     */
    [[nodiscard]] std::vector<Token> scanTokens();

private:
    /**
     * Mapping of keywords to their corresponding token types.
     */
    static const std::unordered_map<std::string, TokenType> keywords;

    /**
     * Source code to scan.
     */
    const std::string source;

    /**
     * List of scanned tokens.
     */
    std::vector<Token> tokens;

    /**
     * Current starting position in the source code.
     */
    int start = 0;

    /**
     * Current position in the source code.
     */
    int current = 0;

    /**
     * Current line number in the source code.
     */
    int line = 1;

    /**
     * Scans a single token from the source code.
     */
    void scanToken();

    /**
     * Adds a token to the list of scanned tokens.
     *
     * @param type Type of the token.
     */
    void addToken(TokenType type);

    /**
     * Adds a token with a literal value to the list of scanned tokens.
     *
     * @param type Type of the token.
     * @param literal Literal value of the token.
     */
    void addToken(TokenType type, const Literal& literal);

    /**
     * Scans a string literal from the source code.
     */
    void string();

    /**
     * Scans a number literal from the source code.
     */
    void number();

    /**
     * Scans an identifier or keyword from the source code.
     */
    void identifier();

    /**
     * Advances the scanner to the next character and returns it.
     *
     * @return The next character in the source code.
     */
    char advance();

    /**
     * Matches the current character with the expected character.
     *
     * @param expected Expected character to match.
     * @return True if the characters match, false otherwise.
     */
    bool match(const char& expected);

    /**
     * Peeks at the current character without advancing the scanner.
     *
     * @return The current character in the source code.
     */
    [[nodiscard]] char peek() const;

    /**
     * Peeks at the next character without advancing the scanner.
     *
     * @return The next character in the source code.
     */
    [[nodiscard]] char peekNext() const;

    /**
     * Checks if the scanner has reached the end of the source code.
     */
    [[nodiscard]] bool isAtEnd() const;

    /**
     * Checks if a character is a digit.
     *
     * @param c Character to check.
     * @return True if the character is a digit, false otherwise.
     */
    [[nodiscard]] bool isDigit(const char& c) const;

    /**
     * Checks if a character is an alphabetic letter or underscore.
     *
     * @param c Character to check
     * @return True if the character is alphabetic or underscore, false otherwise.
     */
    [[nodiscard]] bool isAlpha(const char& c) const;

    /**
     * Checks if a character is alphanumeric (letter or digit).
     *
     * @param c Character to check
     * @return True if the character is alphanumeric, false otherwise.
     */
    [[nodiscard]] bool isAlphaNumeric(const char& c) const;
};
