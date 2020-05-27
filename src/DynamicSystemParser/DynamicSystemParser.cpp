#include <memory>
#include <array>
#include <vector>
#include <string>

#include "DynamicSystemParser/DynamicSystemParser.hpp"

namespace DynamicSystemParser::Impl {

ParserDerivativesWrapper::ParserDerivativesWrapper(std::unique_ptr<std::array<long double, 3>> variables_,
                                                   std::unique_ptr<const Parser::Node> xFunc_,
                                                   std::unique_ptr<const Parser::Node> yFunc_,
                                                   std::unique_ptr<const Parser::Node> zFunc_) :
        variables{std::move(variables_)},
        xFunc{std::move(xFunc_)},
        yFunc{std::move(yFunc_)},
        zFunc{std::move(zFunc_)} {}


ParserDerivativesWrapper parseExpressions(const std::string &xExpr, const std::string &yExpr, const std::string &zExpr,
                                          const std::map<std::string, long double> &customConstVariables) {

    auto variableArray = std::make_unique<std::array<long double, 3>>();
    std::array<long double *, 3> variableAddreses = {&(*variableArray)[0], &(*variableArray)[1], &(*variableArray)[2]};
    std::unique_ptr<const Parser::Node> xFunc, yFunc, zFunc;

    try {
        xFunc = Parser::parseExpression(xExpr, variableAddreses, customConstVariables);
    } catch (const std::exception &exception) {
        throw Parser::ParserException(std::string{"In first expression: "} + std::string{exception.what()});
    }

    try {
        yFunc = Parser::parseExpression(yExpr, variableAddreses, customConstVariables);
    } catch (const std::exception &exception) {
        throw Parser::ParserException(std::string{"In second expression: "} + std::string{exception.what()});
    }

    try {
        zFunc = Parser::parseExpression(zExpr, variableAddreses, customConstVariables);
    } catch (const std::exception &exception) {
        throw Parser::ParserException(std::string{"In third expression: "} + std::string{exception.what()});
    }

    return ParserDerivativesWrapper{std::move(variableArray), std::move(xFunc), std::move(yFunc), std::move(zFunc)};
}

} // namespace DynamicSystemParser::Impl