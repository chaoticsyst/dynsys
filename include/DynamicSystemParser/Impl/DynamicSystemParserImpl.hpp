#pragma once

#include <memory>
#include <vector>
#include <map>
#include <string>

#include "DynamicSystemParser/DynamicSystemParser.hpp"
#include "Parser/Parser.hpp"

namespace DynamicSystemParser {

namespace Impl {

class ParserDerivativesWrapper final {
public:
    ParserDerivativesWrapper(std::unique_ptr<std::array<long double, 3>> variables,
                             std::unique_ptr<const Parser::Node> xFunc,
                             std::unique_ptr<const Parser::Node> yFunc,
                             std::unique_ptr<const Parser::Node> zFunc);

    auto operator()(std::vector<long double> &) {
        return [&xFuncGet = *xFunc, &yFuncGet = *yFunc, &zFuncGet = *zFunc, &variablesGet = *variables] //TODO
                (const Model::Point &point) {
            variablesGet[0] = point.x;
            variablesGet[1] = point.y;
            variablesGet[2] = point.z;
            return Model::Point{xFuncGet.calc(), yFuncGet.calc(), zFuncGet.calc()};
        };
    }

private:
    std::shared_ptr<std::array<long double, 3>> variables;
    std::shared_ptr<const Parser::Node> xFunc, yFunc, zFunc;
};

ParserDerivativesWrapper parseExpressions(const std::string &xExpr, const std::string &yExpr, const std::string &zExpr,
                                          const std::map<std::string, long double> &customConstVariables);

} // namespace Impl

template<typename NewPointAction>
DynamicSystems::DynamicSystem<NewPointAction> getDynamicSystem(
        const std::string &attractorName,
        const std::array<std::string, 3> &formulae,
        const std::vector<std::string> &variablesNames,
        const std::vector<std::pair<std::string, std::vector<long double>>> &interestingConstants,
        const std::map<std::string, long double> &customConstVariables) {
    using DynamicSystem = DynamicSystems::DynamicSystem<NewPointAction>;
    using LambdaDerivatives = decltype(std::declval<Impl::ParserDerivativesWrapper>().operator()(std::declval<std::vector<long double> &>()));
    using DynamicSystemInternal = DynamicSystems::DynamicSystemInternal<NewPointAction, LambdaDerivatives>;
    return DynamicSystem{attractorName, formulae, variablesNames, interestingConstants,
                         DynamicSystemInternal{Impl::parseExpressions(formulae[0], formulae[1], formulae[2], customConstVariables)}};
}

} // namespace DynamicSystemParser
