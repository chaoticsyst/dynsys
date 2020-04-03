#pragma once

#include <string>

namespace Parser {

struct Node {
    virtual long double calc() const noexcept = 0;
};


Node *parse_expression(const std::string &x_expr);

auto parse_expressions(const std::string &x_expr, const std::string &y_expr, const std::string &z_expr);

} // namespace Parser

#include "ParserImpl.h"
