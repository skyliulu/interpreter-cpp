#pragma once
#include "expr.h"
#include <memory>
#include <vector>
#include <any>
class Stmt
{
public:
	Stmt() {}
	~Stmt() {}
	class Expression ;
	class Print ;
	class Var ;
	class Block ;
	class If ;
	class While ;
	class Func ;
	class Return ;
	class Visitor;
	virtual std::any accept(Visitor &visitor) const = 0;
};

class Stmt::Visitor
{
public:
    virtual ~Visitor() = default;
	virtual std::any visit(const Expression  &expr) = 0;
	virtual std::any visit(const Print  &expr) = 0;
	virtual std::any visit(const Var  &expr) = 0;
	virtual std::any visit(const Block  &expr) = 0;
	virtual std::any visit(const If  &expr) = 0;
	virtual std::any visit(const While  &expr) = 0;
	virtual std::any visit(const Func  &expr) = 0;
	virtual std::any visit(const Return  &expr) = 0;
};

class Stmt::Expression  : public Stmt
{
private:
	std::unique_ptr<Expr> expression;
public:
	Expression (std::unique_ptr<Expr> expression) : expression(std::move(expression))
	{
	}
	~Expression () {}
	Expr* get_expression() const { return expression.get(); }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Stmt::Print  : public Stmt
{
private:
	std::unique_ptr<Expr> expression;
public:
	Print (std::unique_ptr<Expr> expression) : expression(std::move(expression))
	{
	}
	~Print () {}
	Expr* get_expression() const { return expression.get(); }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Stmt::Var  : public Stmt
{
private:
	Token name;
	std::unique_ptr<Expr> initializer;
public:
	Var (Token name, std::unique_ptr<Expr> initializer) : name(name), initializer(std::move(initializer))
	{
	}
	~Var () {}
	Token get_name() const { return name; }
	Expr* get_initializer() const { return initializer.get(); }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Stmt::Block  : public Stmt
{
private:
	std::vector<std::unique_ptr<Stmt>> statements;
public:
	Block (std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements))
	{
	}
	~Block () {}
	const std::vector<std::unique_ptr<Stmt>>& get_statements() const { return statements; }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Stmt::If  : public Stmt
{
private:
	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> thenBranch;
	std::unique_ptr<Stmt> elseBranch;
public:
	If (std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch) : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch))
	{
	}
	~If () {}
	Expr* get_condition() const { return condition.get(); }
	Stmt* get_thenBranch() const { return thenBranch.get(); }
	Stmt* get_elseBranch() const { return elseBranch.get(); }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Stmt::While  : public Stmt
{
private:
	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> body;
public:
	While (std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) : condition(std::move(condition)), body(std::move(body))
	{
	}
	~While () {}
	Expr* get_condition() const { return condition.get(); }
	Stmt* get_body() const { return body.get(); }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Stmt::Func  : public Stmt
{
private:
	Token name;
	std::vector<Token> params;
	std::vector<std::unique_ptr<Stmt>> body;
public:
	Func (Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body) : name(name), params(params), body(std::move(body))
	{
	}
	~Func () {}
	Token get_name() const { return name; }
	const std::vector<Token>& get_params() const { return params; }
	const std::vector<std::unique_ptr<Stmt>>& get_body() const { return body; }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Stmt::Return  : public Stmt
{
private:
	Token keyword;
	std::unique_ptr<Expr> value;
public:
	Return (Token keyword, std::unique_ptr<Expr> value) : keyword(keyword), value(std::move(value))
	{
	}
	~Return () {}
	Token get_keyword() const { return keyword; }
	Expr* get_value() const { return value.get(); }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

