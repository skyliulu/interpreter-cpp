#pragma once
#include "callable.h"
#include <memory>
class Instance;

class Function : public Callable {
private:
    /* data */
    const Stmt::Func &declaration;
    std::shared_ptr<Environment> enclosing;
    bool is_init;

public:
    Function(const Stmt::Func &declaration, std::shared_ptr<Environment> enclosing, bool is_init);

    ~Function();

    int arity() override;

    std::shared_ptr<Function> bind(std::shared_ptr<Instance> instance);

    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;


    std::string to_string() const override;
};

class Class : public Callable {
private:
    const std::string name;
    std::shared_ptr<Class> superclass;
    std::unordered_map<std::string, std::shared_ptr<Function> > methods;

public:
    Class(std::string name, std::shared_ptr<Class> superclass,
          std::unordered_map<std::string, std::shared_ptr<Function> > methods);

    ~Class();

    std::string to_string() const override;

    std::shared_ptr<Function> get_method(const std::string &name) const;

    int arity() override;

    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override;
};

class Instance {
private:
    Class &clazz;
    std::unordered_map<std::string, std::any> fields;

public:
    Instance(Class &clazz);

    ~Instance();

    void set(const Token &token, const std::any &value);

    std::any get(const std::shared_ptr<Instance> &instance, const Token &token);

    std::string to_string() const;
};
