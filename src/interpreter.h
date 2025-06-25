#pragma once
#include <iostream>
#include <string>
#include "expr.h"
#include "runtime_error.h"

class Interpreter : public Expr::Visitor
{
private:
    /* data */

    /* helper functions */
    std::any evaluate(const Expr &expr);
    std::string stringify(const std::any &value);
    bool is_truthy(const std::any &value);
    void check_number_operand(const Token &operator_, const std::any &operand);
    void check_number_operands(const Token &operator_, const std::any &left, const std::any &right);

public:
    Interpreter(/* args */);
    ~Interpreter();
    void interpret(const Expr &expr);
    std::any visit(const Expr::Binary &expr) override;
    std::any visit(const Expr::Unary &expr) override;
    std::any visit(const Expr::Literal &expr) override;
    std::any visit(const Expr::Grouping &expr) override;
};
