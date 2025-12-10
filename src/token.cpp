#include "token.h"

#include <format>
#include <magic_enum.hpp>

Token::Token(TokenType type, std::string lexeme, Literal literal, std::size_t line)
    : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

std::string Token::toString() const {
    auto lit = std::visit(
        [](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return "nil";
            } else if constexpr (std::is_same_v<T, std::string>) {
                return arg;
            } else if constexpr (std::is_same_v<T, double>) {
                return std::to_string(arg);
            } else if constexpr (std::is_same_v<T, bool>) {
                return arg ? "true" : "false";
            } else {
                return "unknown";
            }
        },
        literal);

    return std::format("Type: {} | Text: {} | Value: {}", magic_enum::enum_name(type), lexeme, lit);
}
