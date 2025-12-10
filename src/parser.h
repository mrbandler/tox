#include "ast.h"
#include "token.h"
#include "tox.h"

#include <vector>

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
private:
    std::vector<Token> m_tokens;
    std::size_t m_current = 0;

public:
    explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

    ExprPtr parse() {
        try {
            return expression();
        } catch (ParseError error) {
            return nullptr;
        }
    }

private:
    ExprPtr expression() {
        return equality();
    }

    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr primary();

    void synchronize();

    bool match(const std::vector<TokenType>& types);

    Token advance() {
        if (!isAtEnd()) {
            m_current++;
        }

        return previous();
    }

    Token consume(TokenType type, const std::string& msg) {
        if (check(type)) {
            return advance();
        }

        throw error(peek(), msg);
    }

    static ParseError error(const Token& token, const std::string& msg) {
        Tox::error(token.line, msg);

        return ParseError(msg);
    }

    [[nodiscard]] bool check(TokenType type) const {
        if (isAtEnd()) {
            return false;
        }

        return peek().type == type;
    }

    [[nodiscard]] bool isAtEnd() const {
        return peek().type == TokenType::END_OF_FILE;
    }

    [[nodiscard]] Token peek() const {
        return m_tokens[m_current];
    }

    [[nodiscard]] Token previous() const {
        return m_tokens[m_current - 1];
    }
};
