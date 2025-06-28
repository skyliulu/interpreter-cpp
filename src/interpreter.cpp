#include "interpreter.h"
#include "lox.h"
#include "nativefunc.h"
#include <functional>
#include "function.h"

Interpreter::Interpreter(/* args */)
{
    std::shared_ptr<Callable> clock = std::make_shared<Clock>();
    global->define("clock", clock);
}

Interpreter::~Interpreter()
{
}

Interpreter::EnvironmentGuard::EnvironmentGuard(Interpreter &interp, std::shared_ptr<Environment> new_env)
    : interpreter(interp), previous(interp.environment)
{
    interpreter.environment = new_env;
}

Interpreter::EnvironmentGuard::~EnvironmentGuard()
{
    interpreter.environment = previous;
}

void Interpreter::interpret(const Expr &expr)
{
    try
    {
        std::any result = evaluate(expr);
        std::cout << stringify(result) << std::endl;
    }
    catch (const RuntimeError &error)
    {
        Lox::runtime_error(error);
        return; // Exit early if there is a runtime error
    }
}

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>> &stmts)
{
    try
    {
        for (const auto &stmt : stmts)
        {
            execute(*stmt);
        }
    }
    catch (const RuntimeError &e)
    {
        Lox::runtime_error(e);
    }
}

void Interpreter::execute(const Stmt &stmt)
{
    stmt.accept(*this);
}

std::any Interpreter::evaluate(const Expr &expr)
{
    return expr.accept(*this);
}

void Interpreter::execute_block(const std::vector<std::unique_ptr<Stmt>> &stmts, const std::shared_ptr<Environment> &env)
{
    // 使用guard的方式可以保证函数结束时可以将env充值回去
    EnvironmentGuard guard(*this, env); // Create a new environment for the block
    try
    {
        for (const auto &statement : stmts)
        {
            execute(*statement);
        }
    }
    catch (const RuntimeError &e)
    {
        throw; // Re-throw the error
    }
}

void Interpreter::resolve(const Expr &expr, int deep)
{
    locals.emplace(&expr, deep);
}

std::any Interpreter::lookup_var(Token name, const Expr &expr)
{
    if (locals.find(&expr) != locals.end())
    {
        int distance = locals[&expr];
        return environment->get_at(distance, name.get_lexeme());
    }
    else
    {
        return global->get(name);
    }
}

void Interpreter::assign_var(Token name, const Expr &expr, std::any value)
{
    if (locals.find(&expr) != locals.end())
    {
        int distance = locals[&expr];
        return environment->assign_at(distance, name, value);
    }
    else
    {
        return global->assign(name, value);
    }
}

std::any Interpreter::visit(const Stmt::Return &expr)
{
    if (expr.get_value())
    {
        std::any val = evaluate(*(expr.get_value()));
        // std::cout << stringify(val) << std::endl;
        throw Return(val);
    }
    else
    {
        throw Return(std::any());
    }
}

std::any Interpreter::visit(const Stmt::Func &expr)
{
    std::shared_ptr<Callable> function_ptr = std::make_shared<Function>(expr, environment);
    environment->define(expr.get_name().get_lexeme(), function_ptr);
    return function_ptr;
}

std::any Interpreter::visit(const Stmt::Block &expr)
{
    execute_block(expr.get_statements(), std::make_shared<Environment>(environment));
    return std::any(); // Return an empty std::any as the return type is std::any
}

std::any Interpreter::visit(const Stmt::Expression &expr)
{
    return evaluate(*(expr.get_expression()));
}

std::any Interpreter::visit(const Stmt::Print &expr)
{
    std::any value = evaluate(*(expr.get_expression()));
    std::cout << stringify(value) << std::endl;
    return value; // Return the value for potential further use
}

std::any Interpreter::visit(const Stmt::Var &expr)
{
    std::any value;
    if (expr.get_initializer())
    {
        value = evaluate(*(expr.get_initializer()));
    }
    else
    {
        value = nullptr; // If no initializer, set to nil
    }
    environment->define(expr.get_name().get_lexeme(), value); // Define the variable in the environment
    return value;                                             // Return the initialized value
}

std::any Interpreter::visit(const Stmt::While &expr)
{
    while (is_truthy(evaluate(*(expr.get_condition())))) // Continue while the condition is true
    {
        execute(*(expr.get_body())); // Execute the body of the loop
    }
    return std::any(); // Return an empty std::any as the return type is std::any
}

std::any Interpreter::visit(const Stmt::If &expr)
{
    std::any condition = evaluate(*(expr.get_condition()));
    if (is_truthy(condition))
    {
        execute(*(expr.get_thenBranch())); // Execute the then branch if condition is true
    }
    else if (expr.get_elseBranch())
    {
        execute(*(expr.get_elseBranch())); // Execute the else branch if it exists
    }
    return std::any(); // Return an empty std::any as the return type is std::any
}

std::any Interpreter::visit(const Expr::Logical &expr)
{
    std::any left = evaluate(*(expr.get_left()));
    if (expr.get_operator_().get_type() == TokenType::OR)
    {
        if (is_truthy(left))
        {
            return left; // Short-circuit evaluation for OR
        }
    }
    else if (expr.get_operator_().get_type() == TokenType::AND)
    {
        if (!is_truthy(left))
        {
            return left; // Short-circuit evaluation for AND
        }
    }
    return evaluate(*(expr.get_right())); // Evaluate the right side if necessary
}

std::any Interpreter::visit(const Expr::Variable &expr)
{
    return lookup_var(expr.get_name(), expr);
}

std::any Interpreter::visit(const Expr::Assign &expr)
{
    std::any value = evaluate(*(expr.get_value()));
    assign_var(expr.get_name(), expr, value);
    return value;
}

std::any Interpreter::visit(const Expr::Binary &expr)
{
    std::any left = evaluate(*(expr.get_left()));
    std::any right = evaluate(*(expr.get_right()));
    // std::cout << stringify(left) << " " << tokentype_to_string(expr.get_operator_().get_type()) << " " << stringify(right) << std::endl;
    switch (expr.get_operator_().get_type())
    {
    case PLUS:
        if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
        {
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
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
        return equality(left, right);
    case BANG_EQUAL:
        return !equality(left, right);
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

std::any Interpreter::visit(const Expr::Call &expr)
{
    std::any callee = evaluate(*expr.get_callee());
    std::vector<std::any> parmas;
    for (const auto &arg : expr.get_arguments())
    {
        parmas.push_back(evaluate(*arg));
    }
    // std::cout << callee.type().name() << std::endl;
    if (callee.type() == typeid(std::shared_ptr<Callable>))
    {
        std::shared_ptr<Callable> callable = std::any_cast<std::shared_ptr<Callable>>(callee);
        if (parmas.size() != callable->arity())
        {
            throw RuntimeError(expr.get_paren(), "Expect " + std::to_string(callable->arity()) + " arguments but got " + std::to_string(parmas.size()) + ".");
        }
        return callable->call(*this, std::move(parmas));
    }
    else
    {
        throw RuntimeError(expr.get_paren(), "Can only call functions and classes.");
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
    if (value.type() == typeid(std::nullptr_t) || !value.has_value())
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
    else if (value.type() == typeid(std::shared_ptr<Callable>))
    {
        return std::any_cast<std::shared_ptr<Callable>>(value)->to_string();
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

bool Interpreter::equality(const std::any &left, const std::any &right)
{
    if (left.type() != right.type())
    {
        return false; // Different types cannot be equal
    }
    if (left.type() == typeid(double))
    {
        return std::any_cast<double>(left) == std::any_cast<double>(right);
    }
    else if (left.type() == typeid(std::string))
    {
        return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
    }
    else if (left.type() == typeid(bool))
    {
        return std::any_cast<bool>(left) == std::any_cast<bool>(right);
    }
    return false; // For other types, consider them not equal
}

void Interpreter::check_number_operand(const Token &operator_, const std::any &operand)
{
    if (operand.type() != typeid(double))
    {
        throw RuntimeError(operator_, "Operand must be a number");
    }
}

void Interpreter::check_number_operands(const Token &operator_, const std::any &left, const std::any &right)
{
    if (left.type() != typeid(double) || right.type() != typeid(double))
    {
        throw RuntimeError(operator_, "Operands must be numbers");
    }
}