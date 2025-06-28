#pragma once
#include "token.h"
#include <memory>
#include <vector>
#include <any>
class Expr
{
public:
	Expr() {}
	~Expr() {}
	class Binary ;
	class Unary ;
	class Literal ;
	class Grouping ;
	class Variable ;
	class Assign ;
	class Logical ;
	class Call ;
	class Visitor;
	virtual std::any accept(Visitor &visitor) const = 0;
};

class Expr::Visitor
{
public:
    virtual ~Visitor() = default;
	virtual std::any visit(const Binary  &expr) = 0;
	virtual std::any visit(const Unary  &expr) = 0;
	virtual std::any visit(const Literal  &expr) = 0;
	virtual std::any visit(const Grouping  &expr) = 0;
	virtual std::any visit(const Variable  &expr) = 0;
	virtual std::any visit(const Assign  &expr) = 0;
	virtual std::any visit(const Logical  &expr) = 0;
	virtual std::any visit(const Call  &expr) = 0;
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
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
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
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
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
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
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
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Expr::Variable  : public Expr
{
private:
	Token name;
public:
	Variable (Token name) : name(name)
	{
	}
	~Variable () {}
	Token get_name() const { return name; }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Expr::Assign  : public Expr
{
private:
	Token name;
	std::unique_ptr<Expr> value;
public:
	Assign (Token name, std::unique_ptr<Expr> value) : name(name), value(std::move(value))
	{
	}
	~Assign () {}
	Token get_name() const { return name; }
	Expr* get_value() const { return value.get(); }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Expr::Logical  : public Expr
{
private:
	std::unique_ptr<Expr> left;
	Token operator_;
	std::unique_ptr<Expr> right;
public:
	Logical (std::unique_ptr<Expr> left, Token operator_, std::unique_ptr<Expr> right) : left(std::move(left)), operator_(operator_), right(std::move(right))
	{
	}
	~Logical () {}
	Expr* get_left() const { return left.get(); }
	Token get_operator_() const { return operator_; }
	Expr* get_right() const { return right.get(); }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

class Expr::Call  : public Expr
{
private:
	std::unique_ptr<Expr> callee;
	Token paren;
	std::vector<std::unique_ptr<Expr>> arguments;
public:
	Call (std::unique_ptr<Expr> callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments) : callee(std::move(callee)), paren(paren), arguments(std::move(arguments))
	{
	}
	~Call () {}
	Expr* get_callee() const { return callee.get(); }
	Token get_paren() const { return paren; }
	const std::vector<std::unique_ptr<Expr>>& get_arguments() const { return arguments; }
	std::any accept(Visitor &visitor) const override
	{
		return visitor.visit(*this);
	}
};

