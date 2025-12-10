#include "ast.h"

#include <sstream>
#include <string>

std::string ExprPrinter::print(const Expr& expr) {
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
    const Literal& value = expr.value();
    if (std::holds_alternative<std::monostate>(value)) {
        m_output << "nil";
    } else if (std::holds_alternative<std::string>(value)) {
        m_output << std::get<std::string>(value);
    } else if (std::holds_alternative<double>(value)) {
        m_output << std::get<double>(value);
    }
}

void ExprPrinter::visitUnaryExpr(const Unary& expr) {
    m_output << "(";
    m_output << expr.op().lexeme;
    m_output << " ";
    expr.right().accept(*this);
    m_output << ")";
}
