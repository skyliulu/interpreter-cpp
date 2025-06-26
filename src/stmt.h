#pragma once
#include "expr.h"
#include <memory>
#include <any>
class Stmt
{
public:
	Stmt() {}
	~Stmt() {}
	class Expression ;
	class Print ;
	class Visitor;
	virtual std::any accept(Visitor &visitor) const = 0;
};

class Stmt::Visitor
{
public:
    virtual ~Visitor() = default;
	virtual std::any visit(const Expression  &expr) = 0;
	virtual std::any visit(const Print  &expr) = 0;
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

