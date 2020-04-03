#pragma once

#include <string>
#include <exception>

namespace Parser {

auto parse_expressions(const std::string &x_expr, const std::string &y_expr, const std::string &z_expr);


class parse_error : std::exception {
public:
    parse_error(std::string description);

    const char *what() const noexcept override;

private:
    std::string description_;
};

} // namespace Parser

#include "ParserImpl.h"
