#pragma once

#include "ast.h"

#include <any>
#include <stdexcept>
#include <utility>

// Forward declarations.
class Tox;

/**
 * Exception class for runtime errors during interpretation.
 */
class RuntimeError : public std::runtime_error {
private:
    /**
     * The token that caused the runtime error.
     */
    Token m_token;

public:
    /**
     * Construct a new RuntimeError object.
     *
     * @param token The token that caused the runtime error.
     * @param message The error message.
     */
    RuntimeError(Token token, const std::string& message) : std::runtime_error(message), m_token(std::move(token)) {}

    /**
     * Get the token associated with the runtime error.
     *
     * @return The token that caused the runtime error.
     */
    [[nodiscard]] const Token& token() const noexcept {
        return m_token;
    }
};

/**
 * Interpreter class for evaluating expressions.
 */
class Interpreter : public ExprVisitor {
private:
    /**
     * The result of the last evaluated expression.
     */
    std::any m_result;

public:
    /**
     * Interpret an expression and print the result.
     *
     * @param expr The expression to interpret.
     */
    void interpret(const Expr& expr);

    /**
     * Evaluates a given expression and returns the result.
     *
     * @param expr The expression to evaluate.
     * @return The result of the evaluation as std::any.
     */
    [[nodiscard]] std::any eval(const Expr& expr) {
        expr.accept(*this);

        return m_result;
    }

private:
    /**
     * Visit method for the binary expression type.
     *
     * @param expr The binary expression to visit.
     */
    void visitBinaryExpr(const Binary& expr) override;

    /**
     * Visit method for the grouping expression type.
     *
     * @param expr The grouping expression to visit.
     */
    void visitGroupingExpr(const Grouping& expr) override {
        m_result = eval(expr.expression());
    }

    /**
     * Visit method for the literal expression type.
     *
     * @param expr The literal expression to visit.
     */
    void visitLiteralExpr(const Lit& expr) override;

    /**
     * Visit method for the unary expression type.
     *
     * @param expr The unary expression to visit.
     */
    void visitUnaryExpr(const Unary& expr) override;

    /**
     * Check if the operand is a number (double).
     *
     * @param op The operator token for error reporting.
     * @param operand The operand to check.
     * @throws RuntimeError if the operand is not a number.
     */
    static void checkNumberOperand(const Token& op, const std::any& operand) {
        if (operand.type() == typeid(double)) {
            return;
        }

        throw RuntimeError(op, "Operand must be a number.");
    }

    /**
     * Check if both operands are numbers (double).
     *
     * @param op The operator token for error reporting.
     * @param left The left operand to check.
     * @param right The right operand to check.
     * @throws RuntimeError if either operand is not a number.
     */
    static void checkNumberOperands(const Token& op, const std::any& left, const std::any& right) {
        if (left.type() == typeid(double) && right.type() == typeid(double)) {
            return;
        }

        throw RuntimeError(op, "Operands must be numbers.");
    }

    /**
     * Determine the truthiness of a value.
     *
     * @param value The value to evaluate.
     * @return True if the value is considered "truthy", false otherwise.
     */
    [[nodiscard]] static bool isTruthy(const std::any& value);

    /**
     * Check if two std::any values are equal.
     *
     * @param a The first value to compare.
     * @param b The second value to compare.
     * @return True if the values are equal, false otherwise.
     */
    [[nodiscard]] static bool isEqual(const std::any& a, const std::any& b);

    /**
     * Convert a std::any value to its string representation.
     *
     * @param value The value to stringify.
     */
    [[nodiscard]] static std::string stringify(const std::any& value);
};
