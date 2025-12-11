#include "scanner.h"

#include "tox.h"

#include <charconv>
#include <string_view>

Scanner::Scanner(std::string source) : m_source(std::move(source)) {}

[[nodiscard]] std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }

    m_tokens.emplace_back(TokenType::END_OF_FILE, "", std::monostate{}, m_line);
    return m_tokens;
}

const std::unordered_map<std::string, TokenType, StringHash, std::equal_to<>> Scanner::keywords = {
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
        m_line++;
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
            Tox::error(m_line, "Unexpected character.");
        }
        break;
    }
    }
}

void Scanner::addToken(TokenType type) {
    addToken(type, std::monostate{});
}

void Scanner::addToken(TokenType type, const Literal& literal) {
    std::string text = m_source.substr(m_start, m_current - m_start);
    m_tokens.emplace_back(type, text, literal, m_line);
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            m_line++;
        }

        advance();
    }

    if (isAtEnd()) {
        Tox::error(m_line, "Unterminated string.");

        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    auto value = m_source.substr(m_start + 1, m_current - m_start - 2);
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

    double value = 0.0;
    std::from_chars(m_source.data() + m_start, m_source.data() + m_current, value);
    addToken(TokenType::NUMBER, value);
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    std::string_view text(m_source.data() + m_start, m_current - m_start);
    TokenType type = keywords.contains(text) ? keywords.find(text)->second : TokenType::IDENTIFIER;

    addToken(type);
}

char Scanner::advance() {
    return m_source[m_current++];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) {
        return false;
    }

    if (m_source[m_current] != expected) {
        return false;
    }

    m_current++;
    return true;
}

char Scanner::peek() const noexcept {
    if (isAtEnd()) {
        return '\0';
    }

    return m_source[m_current];
}

char Scanner::peekNext() const noexcept {
    if (m_current + 1 >= m_source.length()) {
        return '\0';
    }

    return m_source[m_current + 1];
}

bool Scanner::isAtEnd() const noexcept {
    return m_current >= m_source.length();
}
