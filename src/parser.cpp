#include "parser.h"

#include "ast.h"
#include "token.h"

#include <algorithm>

ExprPtr Parser::equality() {
    auto expr = comparison();

    std::vector<TokenType> types = {TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL};
    while (match(types)) {
        auto op = previous();
        auto right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::comparison() {
    auto expr = term();

    std::vector<TokenType> types = {TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                                    TokenType::LESS_EQUAL};
    while (match(types)) {
        auto op = previous();
        auto right = term();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::term() {
    auto expr = factor();

    std::vector<TokenType> types = {TokenType::MINUS, TokenType::PLUS};
    while (match(types)) {
        auto op = previous();
        auto right = factor();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::factor() {
    auto expr = unary();

    std::vector<TokenType> types = {TokenType::SLASH, TokenType::STAR};
    while (match(types)) {
        auto op = previous();
        auto right = unary();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::unary() {
    std::vector<TokenType> types = {TokenType::BANG, TokenType::MINUS};
    if (match(types)) {
        auto op = previous();
        auto right = unary();
        return std::make_unique<Unary>(std::move(op), std::move(right));
    }

    return primary();
}

ExprPtr Parser::primary() {
    if (match({TokenType::FALSE})) {
        return std::make_unique<Lit>(Literal(false));
    }
    if (match({TokenType::TRUE})) {
        return std::make_unique<Lit>(Literal(true));
    }
    if (match({TokenType::NIL})) {
        return std::make_unique<Lit>(Literal(std::monostate{}));
    }
    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<Lit>(previous().literal);
    }
    if (match({TokenType::LEFT_PAREN})) {
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

bool Parser::match(const std::vector<TokenType>& types) {
    if (std::ranges::any_of(types, [this](const auto& type) { return check(type); })) {
        advance();
        return true;
    }

    return false;
}
