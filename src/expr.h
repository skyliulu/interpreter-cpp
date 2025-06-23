#pragma once
#include "token.h"
#include <memory>
class Expr
{
public:
	Expr() {}
	~Expr() {}
	class Binary ;
	class Unary ;
	class Literal ;
	class Grouping ;
	class Visitor;
	virtual void accept(Visitor &visitor) const = 0;
};

class Expr::Visitor
{
public:
    virtual ~Visitor() = default;
	virtual void visit(const Binary  &expr) = 0;
	virtual void visit(const Unary  &expr) = 0;
	virtual void visit(const Literal  &expr) = 0;
	virtual void visit(const Grouping  &expr) = 0;
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
	void accept(Visitor &visitor) const override
	{
		visitor.visit(*this);
	}
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
	void accept(Visitor &visitor) const override
	{
		visitor.visit(*this);
	}
};

class Expr::Literal  : public Expr
{
private:
	Token keyword;
	LiteralToken value;
public:
	Literal (Token keyword, LiteralToken value) : keyword(keyword), value(value)
	{
	}
	~Literal () {}
	Token get_keyword() const { return keyword; }
	LiteralToken get_value() const { return value; }
	void accept(Visitor &visitor) const override
	{
		visitor.visit(*this);
	}
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
	void accept(Visitor &visitor) const override
	{
		visitor.visit(*this);
	}
};

