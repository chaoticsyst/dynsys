#pragma once

#include <string>
#include <array>

#include "Model.h"

namespace Parser {

struct Node {
    virtual long double calc() const noexcept = 0;
};

/// Order of variable addresses: [x,y,z]
Node *parse_expression(const std::string &expr, const std::array<long double *, 3> &var_address);


inline auto parse_expressions(const std::string &x_expr, const std::string &y_expr, const std::string &z_expr) {
    auto *arr = new std::array<long double, 3>{0, 0, 0};
    std::array<long double *, 3> arr_addresses = {&(*arr)[0], &(*arr)[1], &(*arr)[2]};
    Node *x_func = parse_expression(x_expr, arr_addresses);
    Node *y_func = parse_expression(y_expr, arr_addresses);
    Node *z_func = parse_expression(z_expr, arr_addresses);
    return [x_func, y_func, z_func, arr](const Model::Point &point) {
        (*arr)[0] = point.x;
        (*arr)[1] = point.y;
        (*arr)[2] = point.z;
        return Model::Point{x_func->calc(), y_func->calc(), z_func->calc()};
    };
}

} // namespace Parser
