#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <math.h>
#include "expr.h"

class AstPrinter : public Expr::Visitor, public Stmt::Visitor
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

    void print(std::vector<std::unique_ptr<Stmt>> &statements)
    {
        for (const auto &stmt : statements)
        {
            stmt->accept(*this);
            std::cout << "\n";
        }
    }

    std::any visit(const Stmt::Class &expr) override {
        return {};
    }

    std::any visit(const Stmt::Return &expr) override
    {
        std::cout << "return" << std::endl;
        return std::any();
    }

    std::any visit(const Stmt::Func &expr) override
    {
        return std::any();
    }

    std::any visit(const Stmt::Block &expr) override
    {
        std::cout << "{\n";
        for (const auto &statement : expr.get_statements())
        {
            statement->accept(*this);
            std::cout << "\n";
        }
        std::cout << "}";
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Stmt::Var &expr) override
    {
        std::cout << "var " << expr.get_name().get_lexeme();
        if (expr.get_initializer())
        {
            std::cout << " = ";
            expr.get_initializer()->accept(*this);
        }
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Stmt::Expression &expr) override
    {
        expr.get_expression()->accept(*this);
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Stmt::Print &expr) override
    {
        std::cout << "print ";
        expr.get_expression()->accept(*this);
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Stmt::If &expr) override
    {
        std::cout << "if (";
        expr.get_condition()->accept(*this);
        std::cout << ") ";
        expr.get_thenBranch()->accept(*this);
        if (expr.get_elseBranch())
        {
            std::cout << " else ";
            expr.get_elseBranch()->accept(*this);
        }
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Stmt::While &expr) override
    {
        std::cout << "while (";
        expr.get_condition()->accept(*this);
        std::cout << ") ";
        expr.get_body()->accept(*this);
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Expr::Logical &expr) override
    {
        std::cout << "(" << expr.get_operator_().get_lexeme() << " ";
        expr.get_left()->accept(*this);
        std::cout << " ";
        expr.get_right()->accept(*this);
        std::cout << ")";
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Expr::Assign &expr) override
    {
        std::cout << expr.get_name().get_lexeme() << " = ";
        expr.get_value()->accept(*this);
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Expr::Variable &expr) override
    {
        std::cout << expr.get_name().get_lexeme();
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Expr::Binary &expr) override
    {
        std::cout << "(" << expr.get_operator_().get_lexeme() << " ";
        expr.get_left()->accept(*this);
        std::cout << " ";
        expr.get_right()->accept(*this);
        std::cout << ")";
        return std::any(); // Return an empty std::any as the return type is std::any
    }

    std::any visit(const Expr::Unary &expr) override
    {
        std::cout << "(" << expr.get_operator_().get_lexeme() << " ";
        expr.get_right()->accept(*this);
        std::cout << ")";
        return std::any(); 
    }

    std::any visit(const Expr::Call& expr) override
    {
        return std::any();
    }

    std::any visit(const Expr::Literal &expr) override
    {
        if (std::holds_alternative<std::string>(expr.get_value()))
        {
            std::cout << std::get<std::string>(expr.get_value());
        }
        else if (std::holds_alternative<double>(expr.get_value()))
        {
            std::cout << double_to_string(std::get<double>(expr.get_value()));
        }
        else if (std::holds_alternative<bool>(expr.get_value()))
        {
            std::cout << (std::get<bool>(expr.get_value()) ? "true" : "false");
        }
        else
        {
            std::cout << "nil";
        }
        return std::any(); 
    }

    std::any visit(const Expr::Grouping &expr) override
    {
        std::cout << "(group ";
        expr.get_expression()->accept(*this);
        std::cout << ")";
        return std::any(); 
    }

};
