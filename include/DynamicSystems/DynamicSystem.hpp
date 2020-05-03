#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <functional>

#include "Model.hpp"
#include "DynamicSystemInternal.hpp"


namespace DynamicSystems {


template<typename LambdaNewPointAction>
class DynamicSystem;


template<typename LambdaNewPointAction>
std::vector<DynamicSystem<LambdaNewPointAction>> getDefaultSystems();


template<typename LambdaNewPointAction>
class DynamicSystem {
public:
    template<typename LambdaDerivatives>
    DynamicSystem(std::string attractor_name,
                  std::array<std::string, 3> formulae,
                  std::vector<std::string> variables_names,
                  std::vector<std::pair<std::string, std::vector<long double>>> interesting_constants,
                  DynamicSystemInternal<LambdaNewPointAction, LambdaDerivatives> time_delta);


    std::string_view get_attractor_name() const;

    std::array<std::string_view, 3> get_formulae() const;

    std::vector<std::string_view> get_variables_names() const;

    std::size_t variables_count() const;

    const std::vector<std::pair<std::string, std::vector<long double>>> &get_interesting_constants() const;

private:
    const std::string attractor_name_;
    const std::array<std::string, 3> formulae_;
    const std::vector<std::string> variables_names_;
    const std::vector<std::pair<std::string, std::vector<long double>>> interesting_constants_;
    const std::function<void(LambdaNewPointAction np,
                             Model::Point point,
                             int points_count,
                             int steps_per_point,
                             long double time_delta,
                             std::vector<long double> &con)> compute_;
};


} // namespace DynamicSystem

#include "DynamicSystemImpl.hpp"
#include "SystemsBaseGetImpl.hpp"