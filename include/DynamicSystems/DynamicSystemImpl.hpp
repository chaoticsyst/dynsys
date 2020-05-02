#pragma once

#include "DynamicSystem.hpp"


namespace DynamicSystems {


template<typename LambdaNewPointAction>
template<typename LambdaDerivatives>
DynamicSystem<LambdaNewPointAction>::DynamicSystem(
        std::string attractor_name, std::array<std::string, 3> formulae,
        std::vector<std::string> variables_names,
        std::vector<std::pair<std::string, std::vector<long double>>> interesting_constants,
        DynamicSystemInternal<LambdaNewPointAction, LambdaDerivatives> system) :
        attractor_name_{std::move(attractor_name)},
        formulae_{std::move(formulae)},
        variables_names_{std::move(variables_names)},
        interesting_constants_{std::move(interesting_constants)},
        compute_{
                [system = std::move(system)](LambdaNewPointAction new_point_action,
                                             Model::Point point,
                                             int points_count,
                                             int steps_per_point,
                                             long double time_delta,
                                             std::vector<long double> &variable_value) {
                    system.compute(new_point_action, point, points_count, steps_per_point, time_delta, variable_value);
                }
        } {}


template<typename LambdaNewPointAction>
std::string_view DynamicSystem<LambdaNewPointAction>::get_attractor_name() const {
    return attractor_name_;
}

template<typename LambdaNewPointAction>
std::array<std::string_view, 3> DynamicSystem<LambdaNewPointAction>::get_formulae() const {
    return {formulae_[0], formulae_[1], formulae_[2]};
}

template<typename LambdaNewPointAction>
std::vector<std::string_view> DynamicSystem<LambdaNewPointAction>::get_variables_names() const {
    std::vector<std::string_view> variables_names_view_;
    for (auto &var_name : variables_names_) {
        variables_names_view_.push_back(var_name);
    }
    return variables_names_view_;
}

template<typename LambdaNewPointAction>
std::size_t DynamicSystem<LambdaNewPointAction>::variables_count() const {
    return variables_names_.size();
}

template<typename LambdaNewPointAction>
const std::vector<std::pair<std::string, std::vector<long double>>> &
DynamicSystem<LambdaNewPointAction>::get_interesting_constants() const {
    return interesting_constants_;
}


} // namespace DynamicSystem