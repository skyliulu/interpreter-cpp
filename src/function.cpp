//
// Created by 29043 on 25-6-28.
//
#include "function.h"

Function::Function(const Stmt::Func &declaration, std::shared_ptr<Environment> enclosing,
                   bool is_init) : declaration(declaration),
                                   enclosing(enclosing), is_init(is_init) {
}

::Function::~Function() {
}

int ::Function::arity() {
    return declaration.get_params().size();
}

std::shared_ptr<Function> Function::bind(std::shared_ptr<Instance> instance) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(enclosing);
    env->define("this", instance);
    return std::make_shared<Function>(declaration, env, is_init);
}

std::any Function::call(Interpreter &interpreter, std::vector<std::any> arguments) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(enclosing);
    const std::vector<Token> params = declaration.get_params();
    for (int i = 0; i < arguments.size(); i++) {
        env->define(params.at(i).get_lexeme(), arguments.at(i));
    }

    try {
        interpreter.execute_block(declaration.get_body(), env);
        if (is_init) {
            return enclosing->get_at(0, "this");
        }
        return std::any();
    } catch (const Return &return_stmt) {
        // std::cout << "return from deep " << env->deepth() << std::endl;
        if (is_init) {
            return enclosing->get_at(0, "this");
        }
        return return_stmt.get_value();
    }
}

std::string Function::to_string() const {
    return "<fn " + declaration.get_name().get_lexeme() + ">";
}

::Class::Class(std::string name, std::shared_ptr<Class> superclass,
               std::unordered_map<std::string, std::shared_ptr<Function> > methods) : name(name),superclass(superclass),
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
    if (superclass) {
        return superclass->get_method(name);
    }
    return nullptr;
}

int Class::arity() {
    std::shared_ptr<Function> init = get_method("init");
    return init ? init->arity() : 0;
}

std::any Class::call(Interpreter &interpreter, std::vector<std::any> arguments) {
    std::shared_ptr<Instance> instance = std::make_shared<Instance>(*this);
    std::shared_ptr<Function> init = get_method("init");
    if (init) {
        init->bind(instance)->call(interpreter, arguments);
    }
    return instance;
}

::Instance::Instance(Class &clazz) : clazz(clazz) {
}

::Instance::~Instance() {
    // std::cout << "release instance: " << to_string() << std::endl;
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
        return function;
    }
    throw RuntimeError(token, "Undefined property '" + token.get_lexeme() + "'.");
}

std::string Instance::to_string() const {
    return clazz.to_string() + " instance";
}

std::any handle_property(const std::shared_ptr<Instance> &instance, std::any property) {
    if (property.type() == typeid(std::shared_ptr<Function>)) {
        const std::shared_ptr<Function> function = std::any_cast<std::shared_ptr<Function> >(property);
        return function->bind(instance);
    }
    return property;
}
