#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <math.h>
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

    std::any visit(const Expr::Literal &expr) override
    {
        if (std::holds_alternative<std::string>(expr.get_value()))
        {
            std::cout << std::get<std::string>(expr.get_value());
        }
        else if (std::holds_alternative<double>(expr.get_value()))
        {
            double value = std::get<double>(expr.get_value());
            // Print double with fixed precision
            if(value == std::floor(value))
            {
                // If the value is an integer, print it with one decimal place
                std::cout << std::fixed << std::setprecision(1) << value;
            }
            else
            {
                std::string s = expr.get_keyword().get_lexeme();
                // Remove trailing zeros and the decimal point if it becomes an integer
                // Find the decimal point
                size_t dot_pos = s.find('.');
                if (dot_pos != std::string::npos) {
                    // Remove trailing zeros
                    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
                    // If the last character is now the decimal point, remove it
                    if (s.back() == '.') {
                        s.pop_back();
                    }
                }
                // Otherwise, print it as is
                std::cout << s;
            }
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
