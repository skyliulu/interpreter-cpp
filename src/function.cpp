//
// Created by 29043 on 25-6-28.
//
#include "function.h"

Function::Function(const Stmt::Func &declaration, std::shared_ptr<Environment> enclosing) : declaration(declaration),
    enclosing(enclosing) {
}

::Function::~Function() {
}

int ::Function::arity() {
    return declaration.get_params().size();
}

std::shared_ptr<Callable> Function::bind(Instance instance) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(enclosing);
    env->define("this", instance);
    return std::make_shared<Function>(declaration, env);
}

std::any Function::call(Interpreter &interpreter, std::vector<std::any> arguments) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(enclosing);
    const std::vector<Token> params = declaration.get_params();
    for (int i = 0; i < arguments.size(); i++) {
        env->define(params.at(i).get_lexeme(), arguments.at(i));
    }

    try {
        interpreter.execute_block(declaration.get_body(), env);
        return std::any();
    } catch (const Return &return_stmt) {
        // std::cout << "return from deep " << env->deepth() << std::endl;
        return return_stmt.get_value();
    }
}

std::string Function::to_string() const {
    return "<fn " + declaration.get_name().get_lexeme() + ">";
}

::Class::Class(std::string name,
               std::unordered_map<std::string, std::shared_ptr<Function> > methods) : name(std::move(name)),
    methods(std::move(methods)) {
}

::Class::~Class() {
}

std::string Class::to_string() const {
    return name;
}

std::shared_ptr<Function> Class::get_method(const std::string &name) const {
    if (methods.contains(name)) {
        return methods.at(name);
    }

    return nullptr;
}

int Class::arity() {
    return 0;
}

std::any Class::call(Interpreter &interpreter, std::vector<std::any> arguments) {

    std::shared_ptr<Instance> instance = std::make_shared<Instance>(*this);
    return instance;
}

::Instance::Instance(const Class &clazz) : clazz(clazz) {
}

::Instance::~Instance() {
}

void ::Instance::set(const Token &token, const std::any &value) {
    fields[token.get_lexeme()] = value;
}

std::any Instance::get(const Token &token) {
    if (fields.contains(token.get_lexeme())) {
        return fields[token.get_lexeme()];
    }
    std::shared_ptr<Function> function = clazz.get_method(token.get_lexeme());
    if (function) {
        return function->bind(*this);
    }
    throw RuntimeError(token, "Undefined property '" + token.get_lexeme() + "'.");
}

std::string Instance::to_string() const {
    return clazz.to_string() + " instance";
}
