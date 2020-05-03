#pragma once

#include <string>

#include "ParserException.h"

namespace Parser {

auto parseExpressions(const std::string &x_expr, const std::string &y_expr, const std::string &z_expr);

} // namespace Parser

#include "ParserImpl.hpp"
