#pragma once

#include <string>
#include <map>

#include "ParserException.hpp"

namespace Parser {

struct Node {
    virtual long double calc() const noexcept = 0;

    virtual ~Node() = default;
};

/// Order of variable addresses: [x,y,z]
std::unique_ptr<Node> parseExpression(const std::string &expression,
                                      const std::array<long double *, 3> &variableAddresses,
                                      const std::map<std::string, long double> &customConstVariables = {});

} // namespace Parser
