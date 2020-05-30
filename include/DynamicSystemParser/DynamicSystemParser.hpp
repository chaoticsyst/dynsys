#pragma once

#include <vector>
#include <map>
#include <string>

#include "DynamicSystems/DynamicSystem.hpp"

namespace DynamicSystemParser {

template<typename NewPointAction>
DynamicSystems::DynamicSystem<NewPointAction> getDynamicSystem(
        const std::string &attractorName,
        const std::array<std::string, 3> &formulae,
        const std::vector<std::string> &variablesNames = {},
        const std::vector<std::pair<std::string, std::vector<long double>>> &interestingConstants = {},
        const std::map<std::string, long double> &customConstVariables = {});

} // namespace DynamicSystemParser

#include "DynamicSystemParser/Impl/DynamicSystemParserImpl.hpp"