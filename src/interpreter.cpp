#include "interpreter.h"

Interpreter::Interpreter(/* args */) {}

Interpreter::~Interpreter() {}

void Interpreter::interpret(const Expr &expr)
{
    std::any result = evaluate(expr);
    std::cout << stringify(result) << std::endl;
}

std::any Interpreter::evaluate(const Expr &expr)
{
    return expr.accept(*this);
}

std::any Interpreter::visit(const Expr::Binary &expr)
{
    std::any left = evaluate(*(expr.get_left()));
    std::any right = evaluate(*(expr.get_right()));
    switch (expr.get_operator_().get_type())
    {
    case PLUS:
        if (left.type() == typeid(std::string) || right.type() == typeid(std::string))
        {
            // If either operand is a string, concatenate them
            return stringify(left) + stringify(right);
        }
        if (left.type() == typeid(double) && right.type() == typeid(double))
        {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        }
        throw RuntimeError(expr.get_operator_(), "Operands must be two numbers or two strings for '+' operator");
    case MINUS:
        check_number_operands(expr.get_operator_(), left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
    case STAR:
        check_number_operands(expr.get_operator_(), left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    case SLASH:
        check_number_operands(expr.get_operator_(), left, right);
        if (std::any_cast<double>(right) == 0)
        {
            throw RuntimeError(expr.get_operator_(), "Division by zero");
        }
        return std::any_cast<double>(left) / std::any_cast<double>(right);
    case GREATER:
        check_number_operands(expr.get_operator_(), left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
    case GREATER_EQUAL:
        check_number_operands(expr.get_operator_(), left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
    case LESS:
        check_number_operands(expr.get_operator_(), left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
    case LESS_EQUAL:
        check_number_operands(expr.get_operator_(), left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);
    case EQUAL_EQUAL:
        return is_truthy(left) == is_truthy(right);
    case BANG_EQUAL:
        return is_truthy(left) != is_truthy(right);
    }
}

std::any Interpreter::visit(const Expr::Unary &expr)
{
    std::any right = evaluate(*(expr.get_right()));
    switch (expr.get_operator_().get_type())
    {
    case TokenType::MINUS:
        check_number_operand(expr.get_operator_(), right);
        return -std::any_cast<double>(right);
    case TokenType::BANG:
        return !is_truthy(right);
    default:
        return std::any(); // Should never reach here, but return an empty std::any for safety
    }
}

std::any Interpreter::visit(const Expr::Literal &expr)
{
    // Directly return the value stored in the Literal expression
    std::any value = expr.get_value();
    if (value.type() != typeid(LiteralToken))
    {
        return nullptr; // Return nil
    }
    LiteralToken v = std::any_cast<LiteralToken>(value);

    if (const std::string *str = std::get_if<std::string>(&v))
    {
        return *str; // Return the string value
    }
    else if (const double *num = std::get_if<double>(&v))
    {
        return *num; // Return the double value
    }
    else if (const bool *boolean = std::get_if<bool>(&v))
    {
        return *boolean; // Return the boolean value
    }
    else
    {
        return nullptr; // Return nil
    }
}

std::any Interpreter::visit(const Expr::Grouping &expr)
{
    return evaluate(*(expr.get_expression()));
}

std::string Interpreter::stringify(const std::any &value)
{
    if (value.type() == typeid(std::nullptr_t))
    {
        return "nil";
    }
    else if (value.type() == typeid(double))
    {
        return double_to_string(std::any_cast<double>(value), false);
    }
    else if (value.type() == typeid(std::string))
    {
        return std::any_cast<std::string>(value);
    }
    else if (value.type() == typeid(bool))
    {
        return std::any_cast<bool>(value) ? "true" : "false";
    }
    return "unknown";
}

bool Interpreter::is_truthy(const std::any &value)
{
    if (value.type() == typeid(std::nullptr_t))
    {
        return false; // nil is false
    }
    if (value.type() == typeid(bool))
    {
        return std::any_cast<bool>(value); // true or false
    }
    return true; // All other values are considered true
}

void Interpreter::check_number_operand(const Token &operator_, const std::any &operand)
{
    if (operand.type() != typeid(double))
    {
        throw RuntimeError(operator_, "Operand must be a number'");
    }
}

void Interpreter::check_number_operands(const Token &operator_, const std::any &left, const std::any &right)
{
    if (left.type() != typeid(double) || right.type() != typeid(double))
    {
        throw RuntimeError(operator_, "Operands must be numbers");
    }
}