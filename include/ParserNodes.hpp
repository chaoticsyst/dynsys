#pragma once

#include <cmath>

#include "Parser.hpp"

namespace Parser {

struct NodeVariable final : Node {
    explicit NodeVariable(const long double *variableAddress) :
        value{variableAddress} {}

    long double calc() const noexcept override {
        return *value;
    }

    const long double *value;
};

struct NodeConstant final : Node {
    explicit NodeConstant(long double constantValue) :
        value{constantValue} {}

    long double value;

    long double calc() const noexcept override {
        return value;
    }
};

enum class BinaryOperation {
    Add, Subtract, Multiply, Divide, Power
};

template<BinaryOperation>
struct NodeBinaryOperation : Node {
    NodeBinaryOperation(std::unique_ptr<Node> leftNode, std::unique_ptr<Node> rightNode) :
        left{std::move(leftNode)}, right{std::move(rightNode)} {}

    long double calc() const noexcept override;

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

template<>
long double NodeBinaryOperation<BinaryOperation::Subtract>::calc() const noexcept {
    return left->calc() - right->calc();
}

template<>
long double NodeBinaryOperation<BinaryOperation::Add>::calc() const noexcept {
    return left->calc() + right->calc();
}

template<>
long double NodeBinaryOperation<BinaryOperation::Multiply>::calc() const noexcept {
    return left->calc() * right->calc();
}

template<>
long double NodeBinaryOperation<BinaryOperation::Divide>::calc() const noexcept {
    return left->calc() / right->calc();
}

template<>
long double NodeBinaryOperation<BinaryOperation::Power>::calc() const noexcept {
    return std::pow(left->calc(), right->calc());
}

enum class Function {
    cos, sin, tan,
    acos, asin, atan,
    cosh, sinh, tanh,
    acosh, asinh, atanh,
    exp, sqrt, abs,
    ln, log, negative
};

template<Function>
struct NodeFunction final : Node {
    explicit NodeFunction(std::unique_ptr<Node> argumentNode) :
        argument{std::move(argumentNode)} {}

    long double calc() const noexcept override;

    std::unique_ptr<Node> argument;
};

template<>
long double NodeFunction<Function::cos>::calc() const noexcept {
    return std::cos(argument->calc());
}

template<>
long double NodeFunction<Function::sin>::calc() const noexcept {
    return std::sin(argument->calc());
}

template<>
long double NodeFunction<Function::tan>::calc() const noexcept {
    return std::tan(argument->calc());
}

template<>
long double NodeFunction<Function::acos>::calc() const noexcept {
    return std::acos(argument->calc());
}

template<>
long double NodeFunction<Function::asin>::calc() const noexcept {
    return std::asin(argument->calc());
}

template<>
long double NodeFunction<Function::atan>::calc() const noexcept {
    return std::atan(argument->calc());
}

template<>
long double NodeFunction<Function::cosh>::calc() const noexcept {
    return std::cosh(argument->calc());
}

template<>
long double NodeFunction<Function::sinh>::calc() const noexcept {
    return std::sinh(argument->calc());
}

template<>
long double NodeFunction<Function::tanh>::calc() const noexcept {
    return std::tanh(argument->calc());
}

template<>
long double NodeFunction<Function::acosh>::calc() const noexcept {
    return std::acosh(argument->calc());
}

template<>
long double NodeFunction<Function::asinh>::calc() const noexcept {
    return std::asinh(argument->calc());
}

template<>
long double NodeFunction<Function::atanh>::calc() const noexcept {
    return std::atanh(argument->calc());
}

template<>
long double NodeFunction<Function::exp>::calc() const noexcept {
    return std::exp(argument->calc());
}

template<>
long double NodeFunction<Function::sqrt>::calc() const noexcept {
    return std::sqrt(argument->calc());
}

template<>
long double NodeFunction<Function::abs>::calc() const noexcept {
    return std::abs(argument->calc());
}

template<>
long double NodeFunction<Function::ln>::calc() const noexcept {
    return std::log(argument->calc());
}

template<>
long double NodeFunction<Function::log>::calc() const noexcept {
    return std::log10(argument->calc());
}

template<>
long double NodeFunction<Function::negative>::calc() const noexcept {
    return -(argument->calc());
}

} //namespace Parser
