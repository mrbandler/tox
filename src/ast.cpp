#include "ast.h"

#include <sstream>
#include <string>

[[nodiscard]] std::string ExprPrinter::print(const Expr& expr) {
    m_output.str("");
    m_output.clear();
    expr.accept(*this);
    return m_output.str();
}

void ExprPrinter::visitBinaryExpr(const Binary& expr) {
    m_output << "(";
    m_output << expr.op().lexeme;
    m_output << " ";
    expr.left().accept(*this);
    m_output << " ";
    expr.right().accept(*this);
    m_output << ")";
}

void ExprPrinter::visitGroupingExpr(const Grouping& expr) {
    m_output << "(group ";
    expr.expression().accept(*this);
    m_output << ")";
}

void ExprPrinter::visitLiteralExpr(const Lit& expr) {
    std::visit(
        [this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                m_output << "nil";
            } else if constexpr (std::is_same_v<T, std::string>) {
                m_output << arg;
            } else if constexpr (std::is_same_v<T, double>) {
                m_output << arg;
            } else if constexpr (std::is_same_v<T, bool>) {
                m_output << (arg ? "true" : "false");
            }
        },
        expr.value());
}

void ExprPrinter::visitUnaryExpr(const Unary& expr) {
    m_output << "(";
    m_output << expr.op().lexeme;
    m_output << " ";
    expr.right().accept(*this);
    m_output << ")";
}
