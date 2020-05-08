#pragma once

#include <string>
#include <map>

#include "ParserException.hpp"

namespace Parser {

auto parseExpressions(const std::string &xExpr,
                      const std::string &yExpr,
                      const std::string &zExpr,
                      const std::map<std::string, long double> &customConstVariables = {});

} // namespace Parser

#include "ParserImpl.hpp"
