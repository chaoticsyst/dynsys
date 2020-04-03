#include <memory>
#include <string>
#include <vector>

#include "Parser.h"

namespace Parser {

parse_error::parse_error(std::string description) : description_{std::move(description)} {}

const char *parse_error::what() const noexcept {
    return description_.c_str();
}


namespace {

enum class binary_operation {
    PLUS, MINUS, MULTIPLY, SUBTRACT
};

enum class unary_operation {
    MINUS
};

constexpr std::size_t X_VAR_POS = 0, Y_VAR_POS = 1, Z_VAR_POS = 2;

struct Node_var : Node {
    explicit Node_var(const long double *var_address) : value_{var_address} {}

    const long double *const value_;

    long double calc() const noexcept override {
        return *value_;
    };
};

struct Node_constant : Node {
    explicit Node_constant(long double value) : value_{value} {}

    const long double value_;

    long double calc() const noexcept override {
        return value_;
    };
};

template<unary_operation>
struct Node_unary_operation : Node {
    explicit Node_unary_operation(std::unique_ptr<Node> baby) : baby_{std::move(baby)} {}

    long double calc() const noexcept override;

    const std::unique_ptr<Node> baby_;
};

template<>
long double Node_unary_operation<unary_operation::MINUS>::calc() const noexcept {
    return -baby_->calc();
}

template<binary_operation>
struct Node_binary_operation : Node {
    Node_binary_operation(std::unique_ptr<Node> left, std::unique_ptr<Node> right) : left_{std::move(left)}, right_{std::move(right)} {}

    long double calc() const noexcept override;

    const std::unique_ptr<Node> left_, right_;
};

template<>
long double Node_binary_operation<binary_operation::MINUS>::calc() const noexcept {
    return left_->calc() - right_->calc();
}

template<>
long double Node_binary_operation<binary_operation::PLUS>::calc() const noexcept {
    return left_->calc() + right_->calc();
}

template<>
long double Node_binary_operation<binary_operation::MULTIPLY>::calc() const noexcept {
    return left_->calc() * right_->calc();
}

template<>
long double Node_binary_operation<binary_operation::SUBTRACT>::calc() const noexcept {
    return left_->calc() / right_->calc();
}

struct parse_iterator {
    explicit parse_iterator(const char *string) : pos{string} {}

    char operator*() const {
        return *pos;
    }

    parse_iterator operator++(int) {
        parse_iterator ret{pos};
        ++*this;
        return ret;
    }

    const parse_iterator &operator++() {
        if (*pos != '\0') {
            ++pos;
            while (*pos == ' ' || *pos == '\n') ++pos;
        }
        return *this;
    }

private:
    const char *pos;
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


std::unique_ptr<Node> parse(parse_iterator &iter, const std::array<long double *, 3> &var_address);

std::unique_ptr<Node> read_node(parse_iterator &iter, const std::array<long double *, 3> &var_address) {
    if (*iter == '(') {
        ++iter;
        return parse(iter, var_address);
    }
    if (*iter == '-') {
        ++iter;
        return std::make_unique<Node_unary_operation<unary_operation::MINUS>>(read_node(iter, var_address));
    }
    if (*iter >= '0' && *iter <= '9') {
        long double val = 0;
        while (*iter >= '0' && *iter <= '9') {
            val *= 10;
            val += *iter - '0';
            ++iter;
        }
        if (*iter == '.' || *iter == ',') {
            ++iter;
            for (int i = 1; *iter >= '0' && *iter <= '9' && i < 20; ++i) {
                val += (*iter - '0') * std::pow(10, -i);
                ++iter;
            }
            while (*iter >= '0' && *iter <= '9') {
                ++iter;
            }
        }
        return std::make_unique<Node_constant>(val);
    }
    if (*iter == 'x') {
        ++iter;
        return std::make_unique<Node_var>(var_address[X_VAR_POS]);
    }
    if (*iter == 'y') {
        ++iter;
        return std::make_unique<Node_var>(var_address[Y_VAR_POS]);
    }
    if (*iter == 'z') {
        ++iter;
        return std::make_unique<Node_var>(var_address[Z_VAR_POS]);
    }
    throw parse_error(std::string{"symbol \""} + std::string{*iter} + std::string{"\" is not variable, constant or unary operator"});
}

binary_operation read_binary_operation(parse_iterator &iter) {
    switch (*(iter++)) {
        case '*':
            return binary_operation::MULTIPLY;
        case '/':
            return binary_operation::SUBTRACT;
        case '+':
            return binary_operation::PLUS;
        case '-':
            return binary_operation::MINUS;
        default:
            throw parse_error(std::string{"symbol \""} + std::string{*iter} + std::string{"\" is not binary operator"});
    }
}

void merge_branches(std::vector<std::unique_ptr<Node>> &node_stack, std::vector<binary_operation> &binary_operations_stack) {
    binary_operation op = binary_operations_stack.back();
    binary_operations_stack.pop_back();
    std::unique_ptr<Node> r(node_stack.back().release());
    node_stack.pop_back();
    std::unique_ptr<Node> l(node_stack.back().release());
    node_stack.pop_back();
    switch (op) {
        case binary_operation::PLUS:
            node_stack.push_back(std::make_unique<Node_binary_operation<binary_operation::PLUS>>(std::move(l), std::move(r)));
            break;
        case binary_operation::MINUS:
            node_stack.push_back(std::make_unique<Node_binary_operation<binary_operation::MINUS>>(std::move(l), std::move(r)));
            break;
        case binary_operation::MULTIPLY:
            node_stack.push_back(std::make_unique<Node_binary_operation<binary_operation::MULTIPLY>>(std::move(l), std::move(r)));
            break;
        case binary_operation::SUBTRACT:
            node_stack.push_back(std::make_unique<Node_binary_operation<binary_operation::SUBTRACT>>(std::move(l), std::move(r)));
            break;
    }
}

std::unique_ptr<Node> parse(parse_iterator &iter, const std::array<long double *, 3> &var_address) {
    std::vector<std::unique_ptr<Node>> node_stack;
    std::vector<binary_operation> binary_operations_stack;
    node_stack.push_back(read_node(iter, var_address));
    while (*iter != ')' && *iter != '\0') {
        binary_operation binary_operation = read_binary_operation(iter);
        std::unique_ptr<Node> node = read_node(iter, var_address);
        while (!binary_operations_stack.empty() && get_priority(binary_operation) <= get_priority(binary_operations_stack.back())) {
            merge_branches(node_stack, binary_operations_stack);
        }
        node_stack.push_back(std::move(node));
        binary_operations_stack.push_back(binary_operation);
    }
    while (!binary_operations_stack.empty()) {
        merge_branches(node_stack, binary_operations_stack);
    }
    ++iter;
    return std::move(node_stack[0]);
}

} // namespace

Node *parse_expression(const std::string &expr, const std::array<long double *, 3> &var_address) {
    parse_iterator iter{expr.c_str()};
    return parse(iter, var_address).release();
}

} // namespace Parser