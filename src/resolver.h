#pragma once
#include "expr.h"
#include "interpreter.h"
#include <unordered_map>
#include <vector>
enum ClassType
{
    CLASS_TYPE_NONE,
    CLASS_TYPE_CLASS,
    CLASS_TYPE_SUBCLASS
};

enum FunctionType
{
    FUNC_TYPE_NONE,
    FUNC_TYPE_FUNCTION,
    FUNC_TYPE_METHOD,
    FUNC_TYPE_INITIALIZER
};

enum VariableState
{
    DECLARED,
    DEFINED,
    READ
};

class Variable
{
private:
    /* data */
    Token name;
    VariableState state;

public:
    Variable(Token name, VariableState state);
    ~Variable();
    Token get_name() const;
    VariableState get_state() const;
    void set_state(VariableState state);
};

class Resolver : public Expr::Visitor, public Stmt::Visitor
{
private:
    /* data */
    Interpreter &interpreter;
    std::vector<std::unordered_map<std::string, Variable>> scopes;
    FunctionType current_func = FunctionType::FUNC_TYPE_NONE;
    ClassType current_class = ClassType::CLASS_TYPE_NONE;
    bool in_loop = false;
    /* funcs */
    void beginScope();
    void endScope();
    void declare(Token name);
    void define(Token name);
    void resolve_local(const Expr &expr, Token name, bool isRead);
    void resolve_function(const Stmt::Func &function, FunctionType type);

public:
    Resolver(Interpreter &interpreter);
    ~Resolver();
    void resolve(const std::vector<std::unique_ptr<Stmt>>& stmts);
    void resolve(const Stmt &stmt);
    void resolve(const Expr &expr);
    void resolve(const std::unique_ptr<Expr>& expr);
    // stmt visitor methods
    std::any visit(const Stmt::Expression &expr) override;
    std::any visit(const Stmt::Print &expr) override;
    std::any visit(const Stmt::Var &expr) override;
    std::any visit(const Stmt::Block &expr) override;
    std::any visit(const Stmt::If &expr) override;
    std::any visit(const Stmt::While &expr) override;
    std::any visit(const Stmt::Func &expr) override;
    std::any visit(const Stmt::Return &expr) override;
    // expr visitor methods
    std::any visit(const Expr::Binary &expr) override;
    std::any visit(const Expr::Unary &expr) override;
    std::any visit(const Expr::Literal &expr) override;
    std::any visit(const Expr::Grouping &expr) override;
    std::any visit(const Expr::Variable &expr) override;
    std::any visit(const Expr::Assign &expr) override;
    std::any visit(const Expr::Logical &expr) override;
    std::any visit(const Expr::Call &expr) override;
};
