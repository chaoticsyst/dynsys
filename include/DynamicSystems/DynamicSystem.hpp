#pragma once

#include <vector>
#include <string>
#include <array>
#include <functional>

#include "Model/Model.hpp"
#include "DynamicSystems/Impl/DynamicSystemInternal.hpp"


namespace DynamicSystems {


template<typename LambdaNewPointAction>
class DynamicSystem;


template<typename LambdaNewPointAction>
std::vector<DynamicSystem<LambdaNewPointAction>> getDefaultSystems();


template<typename LambdaNewPointAction>
class DynamicSystem final {
public:
    template<typename LambdaDerivatives>
    DynamicSystem(std::string attractorName,
                  std::array<std::string, 3> formulae,
                  std::vector<std::string> variablesNames,
                  std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants,
                  DynamicSystemInternal<LambdaNewPointAction, LambdaDerivatives> systemInteranal);


    std::string_view getAttractorName() const;

    std::array<std::string_view, 3> getFormulae() const;

    std::vector<std::string_view> getVariablesNames() const;

    std::size_t constantsCount() const;

    const std::vector<std::pair<std::string, std::vector<long double>>> &getInterestingConstants() const;

    const std::function<void(LambdaNewPointAction newPointAction,
                             Model::Point point,
                             int pointsCount,
                             long double timeDelta,
                             std::vector<long double> &constantValues)> compute;

private:
    const std::string attractorName_;
    const std::array<std::string, 3> formulae_;
    const std::vector<std::string> variablesNames_;
    const std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants_;
};


} // namespace DynamicSystem

#include "DynamicSystems/Impl/DynamicSystemImpl.hpp"
#include "DynamicSystems/SystemsBase/SystemsBaseGetImpl.hpp"