#pragma once

#include <functional>
#include <vector>

#include "Model.hpp"


namespace DynamicSystems {


template<typename LambdaNewPointAction, typename LambdaDerivatives>
class DynamicSystemInternal {
public:
    explicit DynamicSystemInternal(
            std::function<LambdaDerivatives(std::vector<long double> &)> derivatives_function_getter) :
            get_derivatives_function{derivatives_function_getter} {}

    void compute(LambdaNewPointAction new_point_action,
                 Model::Point point,
                 int points_count,
                 int steps_per_point,
                 long double time_delta,
                 std::vector<long double> &variable_value) const {
        LambdaDerivatives derivatives_function = get_derivatives_function(variable_value);
        Model::generate_points(new_point_action, point, points_count, steps_per_point, time_delta, derivatives_function);
    }

private:
    const std::function<LambdaDerivatives(std::vector<long double> &)> get_derivatives_function;
};


} // namespace DynamicSystem