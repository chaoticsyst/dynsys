#pragma once

#include <string>

#include "Model.h"
#include "Parser.h"

namespace Parser {

enum class binary_operation {
    PLUS, MINUS, MULTIPLY, SUBTRACT
};

enum class unary_operation {
    MINUS
};

enum class var_type {
    X, Y, Z
};

template<var_type>
struct Node_var : Node {
    static long double value;

    long double calc() const noexcept override {
        return value;
    };
};

template<>
long double Node_var<var_type::X>::value = 0;

template<>
long double Node_var<var_type::Y>::value = 0;

template<>
long double Node_var<var_type::Z>::value = 0;


struct Node_constant : Node {
    explicit Node_constant(long double v) : value{v} {}

    const long double value;

    long double calc() const noexcept override {
        return value;
    };
};

template<unary_operation>
struct Node_unary_operation : Node {
    explicit Node_unary_operation(const Node *node) : baby{node} {}

    long double calc() const noexcept override;

    const Node *baby;
};

template<>
long double Node_unary_operation<unary_operation::MINUS>::calc() const noexcept {
    return -baby->calc();
}

template<binary_operation>
struct Node_binary_operation : Node {
    Node_binary_operation(const Node *left, const Node *right) : l{left}, r{right} {}

    long double calc() const noexcept override;

    const Node *l, *r;
};

template<>
long double Node_binary_operation<binary_operation::MINUS>::calc() const noexcept {
    return l->calc() - r->calc();
}

template<>
long double Node_binary_operation<binary_operation::PLUS>::calc() const noexcept {
    return l->calc() + r->calc();
}

template<>
long double Node_binary_operation<binary_operation::MULTIPLY>::calc() const noexcept {
    return l->calc() * r->calc();
}

template<>
long double Node_binary_operation<binary_operation::SUBTRACT>::calc() const noexcept {
    return l->calc() / r->calc();
}


auto parse_expressions(const std::string &x_expr, const std::string &y_expr, const std::string &z_expr) {
    Node *x_func = parse_expression(x_expr);
    Node *y_func = parse_expression(y_expr);
    Node *z_func = parse_expression(z_expr);
    return [x_func, y_func, z_func](const Model::Point &point) {
        Node_var<var_type::X>::value = point.x;
        Node_var<var_type::Y>::value = point.y;
        Node_var<var_type::Z>::value = point.z;
        return Model::Point{x_func->calc(), y_func->calc(), z_func->calc()};
    };
}

} // namespace Parser
