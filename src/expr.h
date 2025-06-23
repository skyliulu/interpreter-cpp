#pragma once
#include <memory>
#include "token.h"

class Expr
{
public:
	Expr() {}
	~Expr() {}
	class Binary ;
	class Unary ;
	class Literal ;
	class Grouping ;
	template <typename T>
	class Visitor;
	template <typename T>
	T accept(Visitor<T> &visitor)
	{
		return visitor.visit(*this);
	};
};

class Expr::Binary  : public Expr
{
private:
	std::unique_ptr<Expr> left;
	Token operator_;
	std::unique_ptr<Expr> right;
public:
	Binary (std::unique_ptr<Expr> left, Token operator_, std::unique_ptr<Expr> right) : left(std::move(left)), operator_(operator_), right(std::move(right))
	{
	}
	~Binary () {}
	Expr* get_left() const { return left.get(); }
	Token get_operator_() const { return operator_; }
	Expr* get_right() const { return right.get(); }
};

class Expr::Unary  : public Expr
{
private:
	Token operator_;
	std::unique_ptr<Expr> right;
public:
	Unary (Token operator_, std::unique_ptr<Expr> right) : operator_(operator_), right(std::move(right))
	{
	}
	~Unary () {}
	Token get_operator_() const { return operator_; }
	Expr* get_right() const { return right.get(); }
};

class Expr::Literal  : public Expr
{
private:
	LiteralToken value;
public:
	Literal (LiteralToken value) : value(value)
	{
	}
	~Literal () {}
	LiteralToken get_value() const { return value; }
};

class Expr::Grouping  : public Expr
{
private:
	std::unique_ptr<Expr> expression;
public:
	Grouping (std::unique_ptr<Expr> expression) : expression(std::move(expression))
	{
	}
	~Grouping () {}
	Expr* get_expression() const { return expression.get(); }
};

template <typename T>
class Expr::Visitor
{
public:
    virtual ~Visitor() = default;
	virtual T visit(Binary  &expr) = 0;
	virtual T visit(Unary  &expr) = 0;
	virtual T visit(Literal  &expr) = 0;
	virtual T visit(Grouping  &expr) = 0;
};
