#pragma once

#include <string>
#include <variant>

/**
 * Types of tokens.
 */
enum class TokenType : std::uint8_t {
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    END_OF_FILE
};

using Literal = std::variant<std::monostate, std::string, double>;

/**
 * Token representation.
 */
struct Token {
    /**
     * Type of the token.
     */
    TokenType type;

    /**
     * Lexeme (text) of the token.
     */
    std::string lexeme;

    /**
     * Literal value of the token.
     */
    Literal literal;

    /**
     * Line number where the token appears.
     */
    std::size_t line;

    /**
     * Default constructor.
     *
     * @param type Type of the token.
     * @param lexeme Lexeme (text) of the token.
     * @param literal Literal value of the token.
     * @param line Line number where the token appears.
     */
    Token(TokenType type, std::string lexeme, Literal literal, std::size_t line);

    /**
     * Converts the token to a string representation.
     *
     * @return String representation of the token.
     */
    [[nodiscard]] std::string toString() const;
};
