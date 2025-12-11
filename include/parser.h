#pragma once

#include "ast.h"
#include "token.h"

#include <stdexcept>
#include <vector>

// Forward declarations.
class Tox;

/**
 * Exception class for parse errors during parsing.
 */
class ParseError : public std::runtime_error {
public:
    /**
     * Construct a new ParseError object.
     *
     * @param message The error message.
     */
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

/**
 * Parser for tokens to create an abstract syntax tree (AST).
 */
class Parser {
private:
    /**
     * List of tokens to parse.
     */
    std::vector<Token> m_tokens;

    /**
     * Current position in the token list.
     */
    std::size_t m_current = 0;

public:
    /**
     * Constructor for the Parser.
     *
     * @param tokens The list of tokens to parse.
     */
    explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

    /**
     * Parse the tokens and return the expression.
     *
     * @return The parsed expression, or nullptr if parsing failed.
     */
    [[nodiscard]] ExprPtr parse();

private:
    /**
     * Parse an expression.
     *
     * @return The parsed expression.
     */
    [[nodiscard]] ExprPtr expression() {
        return equality();
    }

    /**
     * Parse an equality expression.
     *
     * @return The parsed expression.
     */
    [[nodiscard]] ExprPtr equality();

    /**
     * Parse a comparison expression.
     *
     * @return The parsed expression.
     */
    [[nodiscard]] ExprPtr comparison();

    /**
     * Parse a term expression.
     *
     * @return The parsed expression.
     */
    [[nodiscard]] ExprPtr term();

    /**
     * Parse a factor expression.
     *
     * @return The parsed expression.
     */
    [[nodiscard]] ExprPtr factor();

    /**
     * Parse a unary expression.
     *
     * @return The parsed expression.
     */
    [[nodiscard]] ExprPtr unary();

    /**
     * Parse a primary expression.
     *
     * @return The parsed expression.
     */
    [[nodiscard]] ExprPtr primary();

    /**
     * Synchronize the parser after an error.
     */
    void synchronize();

    /**
     * Matches the current token against multiple types.
     *
     * @param types The token types to match against.
     * @return True if a match was found and the parser advanced, false otherwise.
     */
    template <typename... TokenType>
    bool match(TokenType... types) {
        if ((check(types) || ...)) {
            advance();
            return true;
        }
        return false;
    }

    /**
     * Advance to the next token and return the previous one.
     *
     * @return The previous token.
     */
    const Token& advance() {
        if (!isAtEnd()) {
            m_current++;
        }

        return previous();
    }

    /**
     * Consume a token of the expected type.
     *
     * @param type The expected token type.
     * @param msg The error message if the token does not match.
     * @return The consumed token.
     */
    const Token& consume(TokenType type, const std::string& msg);

    /**
     * Report a parse error at the given token.
     *
     * @param token The token where the error occurred.
     * @param msg The error message.
     * @return A ParseError exception.
     */
    static ParseError error(const Token& token, const std::string& msg);

    /**
     * Check if the current token matches the given type.
     *
     * @param type The token type to check.
     * @return True if the current token matches the type, false otherwise.
     */
    [[nodiscard]] bool check(TokenType type) const noexcept {
        if (isAtEnd()) {
            return false;
        }

        return peek().type == type;
    }

    /**
     * Check if the parser has reached the end of the token list.
     *
     * @return True if at the end, false otherwise.
     */
    [[nodiscard]] bool isAtEnd() const noexcept {
        return peek().type == TokenType::END_OF_FILE;
    }

    /**
     * Peek at the current token without advancing.
     *
     * @return The current token.
     */
    [[nodiscard]] const Token& peek() const noexcept {
        return m_tokens[m_current];
    }

    /**
     * Get the previous token.
     *
     * @return The previous token.
     */
    [[nodiscard]] const Token& previous() const noexcept {
        return m_tokens[m_current - 1];
    }
};
