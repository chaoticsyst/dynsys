#pragma once

#include <string>
#include <cassert>
#include <cmath>
#include <vector>
#include <sstream>
#include <iterator>
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

int get_priority(binary_operation op) {
    switch (op) {
        case binary_operation::PLUS:
        case binary_operation::MINUS:
            return 1;
        case binary_operation::MULTIPLY:
        case binary_operation::SUBTRACT:
            return 2;
    }
}

struct Node {
    virtual long double calc() const = 0;
};

template<var_type>
struct Node_var : Node {
    static long double value;

    long double calc() const override {
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

    long double calc() const override {
        return value;
    };
};

template<unary_operation>
struct Node_unary_operation : Node {
    explicit Node_unary_operation(const Node *node) : baby{node} {}

    long double calc() const override;

    const Node *baby;
};

template<>
long double Node_unary_operation<unary_operation::MINUS>::calc() const {
    return -baby->calc();
}

template<binary_operation>
struct Node_binary_operation : Node {
    Node_binary_operation(const Node *left, const Node *right) : l{left}, r{right} {}

    long double calc() const override;

    const Node *l, *r;
};

template<>
long double Node_binary_operation<binary_operation::MINUS>::calc() const {
    return l->calc() - r->calc();
}

template<>
long double Node_binary_operation<binary_operation::PLUS>::calc() const {
    return l->calc() + r->calc();
}

template<>
long double Node_binary_operation<binary_operation::MULTIPLY>::calc() const {
    return l->calc() * r->calc();
}

template<>
long double Node_binary_operation<binary_operation::SUBTRACT>::calc() const {
    return l->calc() / r->calc();
}

Node *parse(const char *expression, std::size_t &pos);

Node *read_node(const char *s, std::size_t &pos) {
    if (s[pos] == '(') {
        ++pos;
        return parse(s, pos);
    }
    if (s[pos] == '-') {
        ++pos;
        return new Node_unary_operation<unary_operation::MINUS>{read_node(s, pos)};
    }
    if (s[pos] >= '0' && s[pos] <= '9') {
        long double val = 0;
        while (s[pos] >= '0' && s[pos] <= '9') {
            val *= 10;
            val += s[pos] - '0';
            ++pos;
        }
        if (s[pos] == '.' || s[pos] == ',') {
            ++pos;
            for (int i = 1; s[pos] >= '0' && s[pos] <= '9' && i < 20; ++i) {
                val += (s[pos] - '0') * std::pow(10, -i);
                ++pos;
            }
            while (s[pos] >= '0' && s[pos] <= '9') {
                ++pos;
            }
        }
        return new Node_constant{val};
    }
    if (s[pos] == 'x') {
        ++pos;
        return new Node_var<var_type::X>{};
    }
    if (s[pos] == 'y') {
        ++pos;
        return new Node_var<var_type::Y>{};
    }
    if (s[pos] == 'z') {
        ++pos;
        return new Node_var<var_type::Z>{};
    }
    assert(false);
}

binary_operation read_binary_operation(const char *expression, std::size_t &pos) {
    switch (expression[pos++]) {
        case '*':
            return binary_operation::MULTIPLY;
        case '/':
            return binary_operation::SUBTRACT;
        case '+':
            return binary_operation::PLUS;
        case '-':
            return binary_operation::MINUS;
    }
    assert(false);
}

void merge_branches(std::vector<Node *> &node_stack, std::vector<binary_operation> &binary_operations_stack) {
    binary_operation op = binary_operations_stack.back();
    binary_operations_stack.pop_back();
    Node *r = node_stack.back();
    node_stack.pop_back();
    Node *l = node_stack.back();
    node_stack.pop_back();
    switch (op) {
        case binary_operation::PLUS:
            node_stack.push_back(new Node_binary_operation<binary_operation::PLUS>{l, r});
            break;
        case binary_operation::MINUS:
            node_stack.push_back(new Node_binary_operation<binary_operation::MINUS>{l, r});
            break;
        case binary_operation::MULTIPLY:
            node_stack.push_back(new Node_binary_operation<binary_operation::MULTIPLY>{l, r});
            break;
        case binary_operation::SUBTRACT:
            node_stack.push_back(new Node_binary_operation<binary_operation::SUBTRACT>{l, r});
            break;
    }
}

Node *parse(const char *expression, std::size_t &pos) {
    std::vector<Node *> node_stack;
    std::vector<binary_operation> binary_operations_stack;
    node_stack.push_back(read_node(expression, pos));
    while (expression[pos] != ')' && expression[pos] != '\0') {
        binary_operation binop = read_binary_operation(expression, pos);
        Node *node = read_node(expression, pos);
        while (!binary_operations_stack.empty() && get_priority(binop) <= get_priority(binary_operations_stack.back())) {
            merge_branches(node_stack, binary_operations_stack);
        }
        node_stack.push_back(node);
        binary_operations_stack.push_back(binop);
    }
    while (!binary_operations_stack.empty()) {
        merge_branches(node_stack, binary_operations_stack);
    }
    ++pos;
    return node_stack[0];
}

Node *parse_expression(const std::string &x_expr) {
    std::stringstream string_stream{x_expr};
    std::vector<std::string> vec = std::vector<std::string>{std::istream_iterator<std::string>{string_stream},
                                                            std::istream_iterator<std::string>{}};
    std::string x_expr_clear;
    for (std::string &token : vec) {
        x_expr_clear += token;
    }
    std::size_t pos = 0;
    return parse(x_expr_clear.c_str(), pos);
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
