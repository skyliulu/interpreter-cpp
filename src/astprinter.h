#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "expr.h"

class AstPrinter : public Expr::Visitor
{
private:
    /* data */
public:
    AstPrinter(/* args */) {}
    ~AstPrinter() {}

    void print(std::vector<std::unique_ptr<Expr>> &expressions)
    {
        for (const auto &expr : expressions)
        {
            expr->accept(*this);
            std::cout << "\n";
        }
    }

    void visit(const Expr::Binary &expr) override
    {
        std::cout << "(" << " " << expr.get_operator_().get_lexeme() << " ";
        expr.get_left()->accept(*this);
        std::cout << " ";
        expr.get_right()->accept(*this);
        std::cout << " )";
    }

    void visit(const Expr::Unary &expr) override
    {
        std::cout << "(" << expr.get_operator_().get_lexeme() << " ";
        expr.get_right()->accept(*this);
        std::cout << " )";
    }

    void visit(const Expr::Literal &expr) override
    {
        if (std::holds_alternative<std::string>(expr.get_value()))
        {
            std::cout << std::get<std::string>(expr.get_value());
        }
        else if (std::holds_alternative<double>(expr.get_value()))
        {
            std::cout << std::get<double>(expr.get_value());
        }
        else if (std::holds_alternative<bool>(expr.get_value()))
        {
            std::cout << (std::get<bool>(expr.get_value()) ? "true" : "false");
        }
        else
        {
            std::cout << "nil";
        }
    }

    void visit(const Expr::Grouping &expr) override
    {
        std::cout << "( ";
        expr.get_expression()->accept(*this);
        std::cout << ")";
    }

};
