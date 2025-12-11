#include "interpreter.h"

#include "tox.h"

#include <format>
#include <print>
#include <variant>

void Interpreter::interpret(const Expr& expr) {
    try {
        auto value = eval(expr);
        std::println("{}", stringify(value));
    } catch (const RuntimeError& error) {
        Tox::runtimeError(error);
    }
}

void Interpreter::visitLiteralExpr(const Lit& expr) {
    m_result = std::visit([](auto&& v) -> std::any { return v; }, expr.value());
}

void Interpreter::visitUnaryExpr(const Unary& expr) {
    auto right = eval(expr.right());

    switch (expr.op().type) {
    case TokenType::MINUS: {
        checkNumberOperand(expr.op(), right);
        auto value = std::any_cast<double>(right);
        m_result = -value;

        break;
    }
    case TokenType::BANG: {
        m_result = !isTruthy(right);

        break;
    }
    default: // Unreachable.
        m_result = std::monostate{};

        break;
    }
}

void Interpreter::visitBinaryExpr(const Binary& expr) {
    auto left = eval(expr.left());
    auto right = eval(expr.right());

    switch (expr.op().type) {
    case TokenType::PLUS: {
        if (left.type() == typeid(double) && right.type() == typeid(double)) {
            m_result = std::any_cast<double>(left) + std::any_cast<double>(right);
        } else if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
            m_result = std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        } else {
            throw RuntimeError(expr.op(), "Operands must be two numbers or two strings.");
        }

        break;
    }
    case TokenType::MINUS:
        checkNumberOperands(expr.op(), left, right);
        m_result = std::any_cast<double>(left) - std::any_cast<double>(right);

        break;
    case TokenType::STAR:
        checkNumberOperands(expr.op(), left, right);
        m_result = std::any_cast<double>(left) * std::any_cast<double>(right);

        break;
    case TokenType::SLASH:
        checkNumberOperands(expr.op(), left, right);
        m_result = std::any_cast<double>(left) / std::any_cast<double>(right);

        break;

    case TokenType::GREATER:
        checkNumberOperands(expr.op(), left, right);
        m_result = std::any_cast<double>(left) > std::any_cast<double>(right);

        break;

    case TokenType::GREATER_EQUAL:
        checkNumberOperands(expr.op(), left, right);
        m_result = std::any_cast<double>(left) >= std::any_cast<double>(right);

        break;

    case TokenType::LESS:
        checkNumberOperands(expr.op(), left, right);
        m_result = std::any_cast<double>(left) < std::any_cast<double>(right);

        break;

    case TokenType::LESS_EQUAL:
        checkNumberOperands(expr.op(), left, right);
        m_result = std::any_cast<double>(left) <= std::any_cast<double>(right);

        break;

    case TokenType::BANG_EQUAL:
        m_result = !isEqual(left, right);

        break;
    case TokenType::EQUAL_EQUAL:
        m_result = isEqual(left, right);

        break;
    default:
        // Handle error for unsupported binary operator
        break;
    }
}

/**
 * Determine the truthiness of a value.
 *
 * @param value The value to evaluate.
 * @return True if the value is considered "truthy", false otherwise.
 */
[[nodiscard]] bool Interpreter::isTruthy(const std::any& value) {
    if (value.type() == typeid(std::monostate)) {
        return false;
    }

    if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value);
    }

    return true;
}

/**
 * Check if two std::any values are equal.
 *
 * @param a The first value to compare.
 * @param b The second value to compare.
 * @return True if the values are equal, false otherwise.
 */
[[nodiscard]] bool Interpreter::isEqual(const std::any& a, const std::any& b) {
    // Both are nil/monostate
    if (a.type() == typeid(std::monostate) && b.type() == typeid(std::monostate)) {
        return true;
    }

    // One is nil, the other is not
    if (a.type() == typeid(std::monostate) || b.type() == typeid(std::monostate)) {
        return false;
    }

    // Types must match for equality
    if (a.type() != b.type()) {
        return false;
    }

    // Compare based on the actual type
    if (a.type() == typeid(double)) {
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    }
    if (a.type() == typeid(bool)) {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }
    if (a.type() == typeid(std::string)) {
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }

    // Unknown type - can't compare
    return false;
}

[[nodiscard]] std::string Interpreter::stringify(const std::any& value) {
    if (value.type() == typeid(std::monostate)) {
        return "nil";
    }
    if (value.type() == typeid(double)) {
        return std::format("{:g}", std::any_cast<double>(value));
    }
    if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value) ? "true" : "false";
    }
    if (value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(value);
    }

    return "unknown";
}
