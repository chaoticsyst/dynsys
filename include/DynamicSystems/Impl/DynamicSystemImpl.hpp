#pragma once

#include <vector>
#include <string>
#include <array>
#include <utility>

#include "Model/Model.hpp"
#include "DynamicSystems/DynamicSystem.hpp"


namespace DynamicSystems {


template<typename LambdaNewPointAction>
template<typename LambdaDerivatives>
DynamicSystem<LambdaNewPointAction>::DynamicSystem(
        std::string attractorName,
        std::array<std::string, 3> formulae,
        std::vector<std::string> variablesNames,
        std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants,
        DynamicSystemInternal<LambdaNewPointAction, LambdaDerivatives> systemInternal) :
        compute{
                [system = std::move(systemInternal)](LambdaNewPointAction &&newPointAction,
                                                     Model::Point point,
                                                     int pointsCount,
                                                     long double timeDelta,
                                                     std::vector<long double> &variableValue) {
                    system.compute(std::forward<LambdaNewPointAction>(newPointAction),
                                   point, pointsCount, timeDelta, variableValue);
                }
        },
        attractorName_{std::move(attractorName)},
        formulae_{std::move(formulae)},
        variablesNames_{std::move(variablesNames)},
        interestingConstants_{std::move(interestingConstants)} {}


template<typename LambdaNewPointAction>
std::string_view DynamicSystem<LambdaNewPointAction>::getAttractorName() const {
    return attractorName_;
}

template<typename LambdaNewPointAction>
std::array<std::string_view, 3> DynamicSystem<LambdaNewPointAction>::getFormulae() const {
    return {formulae_[0], formulae_[1], formulae_[2]};
}

template<typename LambdaNewPointAction>
std::vector<std::string_view> DynamicSystem<LambdaNewPointAction>::getVariablesNames() const {
    std::vector<std::string_view> variablesNamesView;
    for (auto &varName : variablesNames_) {
        variablesNamesView.push_back(varName);
    }
    return variablesNamesView;
}

template<typename LambdaNewPointAction>
std::size_t DynamicSystem<LambdaNewPointAction>::constantsCount() const {
    return variablesNames_.size();
}

template<typename LambdaNewPointAction>
const std::vector<std::pair<std::string, std::vector<long double>>> &
DynamicSystem<LambdaNewPointAction>::getInterestingConstants() const {
    return interestingConstants_;
}


} // namespace DynamicSystem