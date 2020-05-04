#pragma once

#include <string>
#include <array>
#include <map>
#include <memory>

#include "Model.hpp"
#include "ParserException.h"

namespace Parser {

struct Node {
    virtual long double calc() const noexcept = 0;

    virtual ~Node() = default;
};

/// Order of variable addresses: [x,y,z]
std::shared_ptr<Node>parseExpression(const std::string &expression,
                                      const std::array<long double *, 3> &variableAddresses,
                                      const std::map<std::string, long double> &customConstVariables = {});


inline auto parseExpressions(const std::string &xExpr, const std::string &yExpr, const std::string &zExpr,
                              const std::map<std::string, long double> &customConstVariables) {

    auto variableArray = std::make_shared<std::array<long double, 3>>();
    std::array<long double *, 3> variableAddreses = {&(*variableArray)[0], &(*variableArray)[1], &(*variableArray)[2]};
    std::shared_ptr<const Node> xFunc, yFunc, zFunc;

    try {
        xFunc = parseExpression(xExpr, variableAddreses, customConstVariables);
    } catch (const std::exception &exception) {
        throw ParserException(std::string{"In first expression: "} + std::string{exception.what()});
    }

    try {
        yFunc = parseExpression(yExpr, variableAddreses, customConstVariables);
    } catch (const std::exception &exception) {
        throw ParserException(std::string{"In second expression: "} + std::string{exception.what()});
    }

    try {
        zFunc = parseExpression(zExpr, variableAddreses, customConstVariables);
    } catch (const std::exception &exception) {
        throw ParserException(std::string{"In third expression: "} + std::string{exception.what()});
    }

    return [xFunc, yFunc, zFunc, variableArray](const Model::Point &point) {
        (*variableArray)[0] = point.x;
        (*variableArray)[1] = point.y;
        (*variableArray)[2] = point.z;
        return Model::Point{xFunc->calc(), yFunc->calc(), zFunc->calc()};
    };
}

} // namespace Parser
