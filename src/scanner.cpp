#include "scanner.h"

#include "token.h"
#include "tox.h"

#include <variant>

Scanner::Scanner(std::string source) : source(std::move(source)) {}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "", std::monostate{}, line);
    return tokens;
}

const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and", TokenType::AND},   {"class", TokenType::CLASS}, {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
    {"for", TokenType::FOR},   {"fun", TokenType::FUN},     {"if", TokenType::IF},         {"nil", TokenType::NIL},
    {"or", TokenType::OR},     {"print", TokenType::PRINT}, {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
    {"this", TokenType::THIS}, {"true", TokenType::TRUE},   {"var", TokenType::VAR},       {"while", TokenType::WHILE},
};

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
    case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
    case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
    case '{':
        addToken(TokenType::LEFT_BRACE);
        break;
    case '}':
        addToken(TokenType::RIGHT_BRACE);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case '.':
        addToken(TokenType::DOT);
        break;
    case '-':
        addToken(TokenType::MINUS);
        break;
    case '+':
        addToken(TokenType::PLUS);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;
    case '*':
        addToken(TokenType::STAR);
        break;
    case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '/': {
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd()) {
                advance();
            }
        } else {
            addToken(TokenType::SLASH);
        }

        break;
    }
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;
    case '\n':
        line++;
        break;
    case '"':
        string();
        break;
    default: {
        if (isDigit(c)) {
            number();
        } else if (isAlpha(c)) {
            identifier();
        } else {
            Tox::error(line, "Unexpected character.");
        }
        break;
    }
    }
}

void Scanner::addToken(TokenType type) {
    addToken(type, std::monostate{});
}

void Scanner::addToken(TokenType type, const Literal& literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
        }

        advance();
    }

    if (isAtEnd()) {
        Tox::error(line, "Unterminated string.");

        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    auto value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

void Scanner::number() {
    while (isDigit(peek())) {
        advance();
    }

    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();

        while (isDigit(peek())) {
            advance();
        }
    }

    auto value = std::stod(source.substr(start, current - start));
    addToken(TokenType::NUMBER, value);
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    std::string text = source.substr(start, current - start);
    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;

    addToken(type);
}

char Scanner::advance() {
    return source[current++];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) {
        return false;
    }

    if (source[current] != expected) {
        return false;
    }

    current++;
    return true;
}

char Scanner::peek() const noexcept {
    if (isAtEnd()) {
        return '\0';
    }

    return source[current];
}

char Scanner::peekNext() const noexcept {
    if (current + 1 >= source.length()) {
        return '\0';
    }

    return source[current + 1];
}

bool Scanner::isAtEnd() const noexcept {
    return current >= source.length();
}
