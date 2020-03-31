#pragma once

#include <functional>
#include <string>
#include "Model.h"

namespace Parser {

auto parse_expressions(const std::string &x_expr, const std::string &y_expr, const std::string &z_expr);

} // namespace Parser

#include "ParserImpl.h"