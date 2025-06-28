#include "resolver.h"
#include "lox.h"

Variable::Variable(Token name, VariableState state) : name(name), state(state) {}
Variable::~Variable() = default;
Token Variable::get_name() const
{
    return name;
}
VariableState Variable::get_state() const
{
    return state;
}
void Variable::set_state(VariableState state)
{
    this->state = state;
}

Resolver::Resolver(Interpreter &interpreter) : interpreter(interpreter)
{
}

Resolver::~Resolver()
{
}
void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>> &stmts)
{
    for (const auto &stmt : stmts)
    {
        stmt->accept(*this);
    }
}
void Resolver::resolve(const Stmt &stmt)
{
    stmt.accept(*this);
}
void Resolver::resolve(const Expr &expr)
{
    expr.accept(*this);
}
void Resolver::resolve(const std::unique_ptr<Expr> &expr)
{
    expr->accept(*this);
}
// stmt visitor methods
std::any Resolver::visit(const Stmt::Expression &expr)
{
    resolve(*expr.get_expression());
    return {};
}
std::any Resolver::visit(const Stmt::Print &expr)
{
    resolve(*expr.get_expression());
    return {};
}
std::any Resolver::visit(const Stmt::Var &expr)
{
    declare(expr.get_name());
    if (expr.get_initializer())
    {
        resolve(*expr.get_initializer());
    }
    define(expr.get_name());
    return {};
}
std::any Resolver::visit(const Stmt::Block &expr)
{
    beginScope();
    resolve(expr.get_statements());
    endScope();
    return {};
}
std::any Resolver::visit(const Stmt::If &expr)
{
    resolve(*expr.get_condition());
    resolve(*expr.get_thenBranch());
    if (expr.get_elseBranch())
    {
        resolve(*expr.get_elseBranch());
    }
    return {};
}
std::any Resolver::visit(const Stmt::While &expr)
{
    bool is_in_loop = in_loop;
    in_loop = true;
    resolve(*expr.get_condition());
    resolve(*expr.get_body());
    in_loop = is_in_loop;
    return {};
}
std::any Resolver::visit(const Stmt::Func &expr)
{
    declare(expr.get_name());
    define(expr.get_name());
    resolve_function(expr, FunctionType::FUNC_TYPE_FUNCTION);
    return {};
}
std::any Resolver::visit(const Stmt::Return &expr)
{
    if (current_func == FunctionType::FUNC_TYPE_NONE)
    {
        Lox::error(expr.get_keyword(), "Can't return from top-level code.");
    }
    if (expr.get_value())
    {
        if (current_func == FunctionType::FUNC_TYPE_INITIALIZER)
        {
            Lox::error(expr.get_keyword(), "Can't return a value from an initializer.");
        }
        resolve(*expr.get_value());
    }

    return {};
}
std::any Resolver::visit(const Stmt::Class &expr) {
    declare(expr.get_name());
    define(expr.get_name());
    if (expr.get_super_class()) {
        if (expr.get_super_class()->get_name().get_lexeme() == expr.get_name().get_lexeme()) {
            Lox::error(expr.get_name(), "A class can't inherit from itself.");
        }
        resolve(*expr.get_super_class());
        beginScope();
        scopes.back().emplace("super", Variable(expr.get_super_class()->get_name(), READ));
    }

    ClassType enclosing = current_class;
    current_class = expr.get_super_class() ? CLASS_TYPE_SUBCLASS : CLASS_TYPE_CLASS;
    beginScope();
    scopes.back().emplace("this", Variable(expr.get_name(), READ));
    for (const auto& method : expr.get_methods()) {
        FunctionType func_type = method->get_name().get_lexeme() == "init" ? FunctionType::FUNC_TYPE_INITIALIZER : FunctionType::FUNC_TYPE_METHOD;
        resolve_function(*method, func_type);
    }
    endScope();
    if (expr.get_super_class()) {
        endScope();
    }
    current_class = enclosing;
    return {};
}
// expr visitor methods
std::any Resolver::visit(const Expr::Binary &expr)
{
    resolve(*expr.get_left());
    resolve(*expr.get_right());
    return {};
}
std::any Resolver::visit(const Expr::Unary &expr)
{
    resolve(*expr.get_right());
    return {};
}
std::any Resolver::visit(const Expr::Literal &expr)
{
    return {};
}
std::any Resolver::visit(const Expr::Grouping &expr)
{
    resolve(*expr.get_expression());
    return {};
}
std::any Resolver::visit(const Expr::Variable &expr)
{
    if (!scopes.empty())
    {
        std::unordered_map<std::string, Variable> &latest_scope = scopes.back();
        if (latest_scope.find(expr.get_name().get_lexeme()) != latest_scope.end())
        {
            Variable variable = latest_scope.at(expr.get_name().get_lexeme());
            if (variable.get_state() == VariableState::DECLARED)
            {
                Lox::error(expr.get_name(), "Can't read local variable in its own initializer.");
            }
        }
    }
    resolve_local(expr, expr.get_name(), true);
    return {};
}
std::any Resolver::visit(const Expr::Assign &expr)
{
    resolve(*expr.get_value());
    resolve_local(expr, expr.get_name(), false);
    return {};
}
std::any Resolver::visit(const Expr::Logical &expr)
{
    resolve(*expr.get_left());
    resolve(*expr.get_right());
    return {};
}
std::any Resolver::visit(const Expr::Call &expr)
{
    resolve(*expr.get_callee());
    for (const auto &exp : expr.get_arguments())
    {
        resolve(exp);
    }
    return {};
}

std::any Resolver::visit(const Expr::Get &expr) {
    resolve(*expr.get_object());
    return {};
}

std::any Resolver::visit(const Expr::Set &expr) {
    resolve(*expr.get_object());
    resolve(*expr.get_value());
    return {};
}

std::any Resolver::visit(const Expr::This &expr) {
    if (current_class == CLASS_TYPE_NONE) {
        Lox::error(expr.get_keyword(), "Can't use 'this' outside of class.");
        return {};
    }
    resolve_local(expr, expr.get_keyword(), true);
    return {};
}

std::any Resolver::visit(const Expr::Super &expr) {
    if (current_class == CLASS_TYPE_NONE) {
        Lox::error(expr.get_keyword(), "Can't use 'super' outside of class.");
    } else if (current_class == CLASS_TYPE_CLASS) {
        Lox::error(expr.get_keyword(), "Can't use 'super' outside of class.");
    }
    resolve_local(expr, expr.get_keyword(), true);
    return {};
}

void Resolver::beginScope()
{
    scopes.emplace_back();
}
void Resolver::endScope()
{
    std::unordered_map<std::string, Variable> &scope = scopes.back();
    scopes.pop_back();
    // for (const auto &pair : scope)
    // {
    //     if (pair.second.get_state() != VariableState::READ)
    //     {
    //         Lox::error(pair.second.get_name(), "Local variable is not used.");
    //     }
    // }
}
void Resolver::declare(Token name)
{
    if (scopes.empty())
    {
        return;
    }
    std::unordered_map<std::string, Variable> &scope = scopes.back();
    if (scope.find(name.get_lexeme()) != scope.end())
    {
        Lox::error(name, "Already a variable with this name in this scope.");
    }
    scope.emplace(name.get_lexeme(), Variable(name, VariableState::DECLARED));
}
void Resolver::define(Token name)
{
    if (scopes.empty())
    {
        return;
    }
    scopes.back().at(name.get_lexeme()).set_state(VariableState::DEFINED);
}
void Resolver::resolve_local(const Expr &expr, Token name, bool isRead)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        auto it = scopes[i].find(name.get_lexeme());
        if (it != scopes[i].end())
        {
            interpreter.resolve(expr, scopes.size() - i - 1);
            if (isRead)
            {
                it->second.set_state(VariableState::READ);
            }
            return;
        }
    }
}
void Resolver::resolve_function(const Stmt::Func &function, FunctionType type)
{
    FunctionType enclosing = current_func;
    current_func = type;
    beginScope();
    for (const auto &token : function.get_params())
    {
        declare(token);
        define(token);
    }
    resolve(function.get_body());
    endScope();
    current_func = enclosing;
}