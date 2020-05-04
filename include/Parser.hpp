#pragma once

#include <string>

#include "ParserException.h"

namespace Parser {

auto parseExpressions(const std::string &xExpr, const std::string &yExpr, const std::string &zExpr);

} // namespace Parser

#include "ParserImpl.hpp"
