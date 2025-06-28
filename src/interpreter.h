#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "stmt.h"
#include "runtime_error.h"
#include "environment.h"
#include <unordered_map>

class Interpreter : public Expr::Visitor, public Stmt::Visitor
{
private:
    /* data */
    std::shared_ptr<Environment> global = std::make_shared<Environment>();
    std::shared_ptr<Environment> environment = global; // The environment to store variable values
    /* helper functions */
    void execute(const Stmt &stmt);
    std::any evaluate(const Expr &expr);
    std::string stringify(const std::any &value);
    bool is_truthy(const std::any &value);
    void check_number_operand(const Token &operator_, const std::any &operand);
    void check_number_operands(const Token &operator_, const std::any &left, const std::any &right);
    bool equality(const std::any &left, const std::any &right);

    class EnvironmentGuard
    {
    private:
        Interpreter &interpreter;
        std::shared_ptr<Environment> previous;

    public:
        EnvironmentGuard(Interpreter &interp, std::shared_ptr<Environment> new_env);
        ~EnvironmentGuard();
        EnvironmentGuard(const EnvironmentGuard &) = delete;
        EnvironmentGuard &operator=(const EnvironmentGuard &) = delete;
    };
    std::unordered_map<const Expr*, int> locals;
    std::any lookup_var(Token name, const Expr& expr);
    void assign_var(Token name, const Expr& expr, std::any value);

public:
    Interpreter(/* args */);
    ~Interpreter();
    void interpret(const Expr &expr);
    void interpret(const std::vector<std::unique_ptr<Stmt>> &stmts);
    void execute_block(const std::vector<std::unique_ptr<Stmt>> &stmts, const std::shared_ptr<Environment> &env);
    void resolve(const Expr &expr, int deep);
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
