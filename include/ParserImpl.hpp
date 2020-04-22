#pragma once

#include <string>
#include <array>
#include <memory>

#include "Model.hpp"

namespace Parser {

struct Node {
    virtual long double calc() const noexcept = 0;

    virtual ~Node() = default;
};

/// Order of variable addresses: [x,y,z]
std::shared_ptr<Node> parse_expression(const std::string &expr, const std::array<long double *, 3> &var_address);


inline auto parse_expressions(const std::string &x_expr, const std::string &y_expr, const std::string &z_expr) {
    auto variables_array = std::make_shared<std::array<long double, 3>>();
    std::array<long double *, 3> variables_addresses = {&(*variables_array)[0], &(*variables_array)[1], &(*variables_array)[2]};
    std::shared_ptr<const Node> x_func, y_func, z_func;

    try {
        x_func = parse_expression(x_expr, variables_addresses);
    } catch (const parse_error &exception) {
        throw parse_error(std::string{"In first expression: "} + std::string{exception.what()});
    }

    try {
        y_func = parse_expression(y_expr, variables_addresses);
    } catch (const parse_error &exception) {
        throw parse_error(std::string{"In second expression: "} + std::string{exception.what()});
    }

    try {
        z_func = parse_expression(z_expr, variables_addresses);
    } catch (const parse_error &exception) {
        throw parse_error(std::string{"In third expression: "} + std::string{exception.what()});
    }

    return [x_func, y_func, z_func, variables_array](const Model::Point &point) {
        (*variables_array)[0] = point.x;
        (*variables_array)[1] = point.y;
        (*variables_array)[2] = point.z;
        return Model::Point{x_func->calc(), y_func->calc(), z_func->calc()};
    };
}

} // namespace Parser
