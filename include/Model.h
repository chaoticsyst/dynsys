#pragma once

#include <functional>

namespace Model {

struct Point {
    long double x, y, z;
};

template<typename LambdaDerivatives, typename LambdaNewPointAction>
void generate_points(LambdaNewPointAction new_point_action,
                     Point point,
                     int points_count,
                     int steps_per_point,
                     long double tau,
                     LambdaDerivatives count_derivatives);

} // namespace Model

#include "DynamicSystemsDefault.hpp"
#include "ModelImpl.hpp"