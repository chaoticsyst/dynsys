#pragma once

namespace Model {

struct Point {
    long double x, y, z;
};

template<typename LambdaDerivatives, typename LambdaNewPointAction>
void generatePoints(LambdaNewPointAction newPointAction,
                    Point point,
                    int pointsCount,
                    long double tau,
                    LambdaDerivatives countDerivatives);

} // namespace Model

#include "ModelImpl.hpp"