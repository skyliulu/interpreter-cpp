#pragma once
#include "callable.h"
#include <memory>
class Instance;
class Function : public Callable
{
private:
    /* data */
    const Stmt::Func &declaration;
    std::shared_ptr<Environment> enclosing;

public:
    Function(const Stmt::Func &declaration, std::shared_ptr<Environment> enclosing) ;
    ~Function() ;
    int arity() override;
    std::shared_ptr<Function> bind(Instance instance);

    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;


    std::string to_string() const override;
};

class Class {
private:
    const std::string name;
    std::unordered_map<std::string, std::shared_ptr<Function> > methods;

public:
    Class(std::string name,
          std::unordered_map<std::string, std::shared_ptr<Function> > methods) ;

    ~Class();

    std::string to_string() const;
    std::shared_ptr<Function> get_method(const std::string &name) const ;
};

class Instance {
private:
    const Class &clazz;
    std::unordered_map<std::string, std::any> fields;

public:
    explicit Instance(const Class &clazz) ;

    ~Instance();

    void set(const Token &token, const std::any &value);

    std::any get(const Token &token);
};
