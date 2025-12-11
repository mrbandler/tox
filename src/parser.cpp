#include "parser.h"

#include "tox.h"

[[nodiscard]] ExprPtr Parser::parse() {
    try {
        return expression();
    } catch (const ParseError& error) {
        return nullptr;
    }
}

const Token& Parser::consume(TokenType type, const std::string& msg) {
    if (check(type)) {
        return advance();
    }

    throw error(peek(), msg);
}

ParseError Parser::error(const Token& token, const std::string& msg) {
    Tox::error(token, msg);

    return ParseError(msg);
}

ExprPtr Parser::equality() {
    auto expr = comparison();

    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        auto op = previous();
        auto right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::comparison() {
    auto expr = term();

    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)) {
        auto op = previous();
        auto right = term();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::term() {
    auto expr = factor();

    while (match(TokenType::MINUS, TokenType::PLUS)) {
        auto op = previous();
        auto right = factor();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::factor() {
    auto expr = unary();

    while (match(TokenType::SLASH, TokenType::STAR)) {
        auto op = previous();
        auto right = unary();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::unary() {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        auto op = previous();
        auto right = unary();
        return std::make_unique<Unary>(std::move(op), std::move(right));
    }

    return primary();
}

ExprPtr Parser::primary() {
    if (match(TokenType::FALSE)) {
        return std::make_unique<Lit>(Literal(false));
    }
    if (match(TokenType::TRUE)) {
        return std::make_unique<Lit>(Literal(true));
    }
    if (match(TokenType::NIL)) {
        return std::make_unique<Lit>(Literal(std::monostate{}));
    }
    if (match(TokenType::NUMBER, TokenType::STRING)) {
        return std::make_unique<Lit>(previous().literal);
    }
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) {
            return;
        }

        switch (peek().type) {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        default:
            break;
        }

        advance();
    }
}
