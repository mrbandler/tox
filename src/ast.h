#pragma once
#include "token.h"

#include <memory>
#include <sstream>
#include <string>

// Forward declarations
class Expr;
class Binary;
class Grouping;
class Lit;
class Unary;

/**
 * Expression Visitor interface for the Visitor pattern.
 * This interface declares visit methods for each concrete expression type.
 */
class ExprVisitor {
public:
    /**
     * Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~ExprVisitor() = default;

    /**
     * Visit methods for the binary expression type.
     *
     * @param expr The binary expression to visit.
     */
    virtual void visitBinaryExpr(const Binary& expr) = 0;

    /**
     * Visit methods for the grouping expression type.
     *
     * @param expr The grouping expression to visit.
     */
    virtual void visitGroupingExpr(const Grouping& expr) = 0;

    /**
     * Visit methods for the literal expression type.
     *
     * @param expr The literal expression to visit.
     */
    virtual void visitLiteralExpr(const Lit& expr) = 0;

    /**
     * Visit methods for the unary expression type.
     *
     * @param expr The unary expression to visit.
     */
    virtual void visitUnaryExpr(const Unary& expr) = 0;
};

/**
 * Expression printer implementation using the ExprVisitor interface.
 * Prints expressions in a Lisp-like parenthesized format.
 */
class ExprPrinter : public ExprVisitor {
private:
    /**
     * String stream to build the output string.
     */
    std::ostringstream m_output;

public:
    /**
     * Print an expression and return its string representation.
     *
     * @param expr The expression to print.
     * @return The string representation of the expression.
     */
    std::string print(const Expr& expr);

    void visitBinaryExpr(const Binary& expr) override;
    void visitGroupingExpr(const Grouping& expr) override;
    void visitLiteralExpr(const Lit& expr) override;
    void visitUnaryExpr(const Unary& expr) override;
};

/**
 * Abstract base class for all expression types in the AST.
 */
class Expr {
public:
    /**
     * Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~Expr() = default;

    /**
     * Accept method for the Visitor pattern.
     *
     * @param visitor The expression visitor.
     */
    virtual void accept(ExprVisitor& visitor) const = 0;
};

/**
 * Expression pointer type for managing expression objects.
 */
using ExprPtr = std::unique_ptr<Expr>;

/**
 * Binary expression class representing binary operations.
 */
class Binary : public Expr {
private:
    /**
     * Left operand of the binary expression.
     */
    ExprPtr m_left;

    /**
     * Operator token of the binary expression.
     */
    Token m_op;

    /*
     * Right operand of the binary expression.
     */
    ExprPtr m_right;

public:
    /**
     * Constructor for the Binary expression.
     *
     * @param left  The left operand expression.
     * @param op    The operator token.
     * @param right The right operand expression.
     */
    Binary(ExprPtr left, Token op, ExprPtr right)
        : m_left(std::move(left)), m_op(std::move(op)), m_right(std::move(right)) {}

    /**
     * Get the left operand expression.
     *
     * @return The left operand expression.
     */
    [[nodiscard]] const Expr& left() const {
        return *m_left;
    }

    /**
     * Get the operator token.
     *
     * @return The operator token.
     */
    [[nodiscard]] const Token& op() const {
        return m_op;
    }

    /**
     * Get the right operand expression.
     *
     * @return The right operand expression.
     */
    [[nodiscard]] const Expr& right() const {
        return *m_right;
    }

    /**
     * Accept method for the Visitor pattern.
     *
     * @param visitor The expression visitor.
     */
    void accept(ExprVisitor& visitor) const override {
        visitor.visitBinaryExpr(*this);
    }
};

/**
 * Grouping expression class representing grouped expressions.
 */
class Grouping : public Expr {
private:
    /**
     * The expression contained within the grouping.
     */
    ExprPtr m_expression;

public:
    /**
     * Constructor for the Grouping expression.
     *
     * @param expression The expression to be grouped.
     */
    Grouping(ExprPtr expression) : m_expression(std::move(expression)) {}

    /**
     * Get the contained expression.
     *
     * @return The contained expression.
     */
    [[nodiscard]] const Expr& expression() const {
        return *m_expression;
    }

    /**
     * Accept method for the Visitor pattern.
     *
     * @param visitor The expression visitor.
     */
    void accept(ExprVisitor& visitor) const override {
        visitor.visitGroupingExpr(*this);
    }
};

/**
 * Literal expression class representing literal values.
 */
class Lit : public Expr {
private:
    /**
     * The literal value.
     */
    Literal m_value;

public:
    /**
     * Constructor for the Literal expression.
     */
    Lit(Literal value) : m_value(std::move(value)) {}

    /**
     * Get the literal value.
     *
     * @return The literal value.
     */
    [[nodiscard]] const Literal& value() const {
        return m_value;
    }

    /**
     * Accept method for the Visitor pattern.
     *
     * @param visitor The expression visitor.
     */
    void accept(ExprVisitor& visitor) const override {
        visitor.visitLiteralExpr(*this);
    }
};

/**
 * Unary expression class representing unary operations.
 */
class Unary : public Expr {
private:
    /**
     * Operator token of the unary expression.
     */
    Token m_op;

    /**
     * Operand of the unary expression.
     */
    ExprPtr m_right;

public:
    /**
     *   * Constructor for the Unary expression.
     *
     * @param op    The operator token.
     * @param right The operand expression.
     */
    Unary(Token op, ExprPtr right) : m_op(std::move(op)), m_right(std::move(right)) {}

    /**
     * Get the operator token.
     *
     * @return The operator token.
     */
    [[nodiscard]] const Token& op() const {
        return m_op;
    }

    /**
     * Get the operand expression.
     *
     * @return The operand expression.
     */
    [[nodiscard]] const Expr& right() const {
        return *m_right;
    }

    /**
     * Accept method for the Visitor pattern.
     *
     * @param visitor The expression visitor.
     */
    void accept(ExprVisitor& visitor) const override {
        visitor.visitUnaryExpr(*this);
    }
};
