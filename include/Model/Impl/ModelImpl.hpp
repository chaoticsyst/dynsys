#pragma once

#include <utility>
#include <cmath>

#include "Model/Model.hpp"

namespace Model {

namespace Impl {

constexpr long double COORDINATE_VALUE_LIMIT = 1e3;

template<typename LambdaNextPointGenerator, typename LambdaNewPointAction>
void generatePointsMainloop(LambdaNewPointAction &&newPointAction,
                            Point point,
                            int pointsCount,
                            LambdaNextPointGenerator &&nextPoint) {
    for (int i = 0; i < pointsCount &&
                    std::abs(point.x) < COORDINATE_VALUE_LIMIT &&
                    std::abs(point.y) < COORDINATE_VALUE_LIMIT &&
                    std::abs(point.z) < COORDINATE_VALUE_LIMIT; ++i) {
        newPointAction(point = nextPoint(point));
    }
}


template<typename LambdaDerivatives, typename LambdaNewPointAction>
void setNextPointGenerator(LambdaNewPointAction &&newPointAction,
                           Point point,
                           int pointsCount,
                           long double tau,
                           LambdaDerivatives &&countDerivatives) {
    auto nextPoint = [tau, countDerivatives = std::forward<LambdaDerivatives>(countDerivatives)]
            (const Point &point) {
        Point k1 = countDerivatives(point);
        Point send{
                point.x + k1.x * (tau / 2),
                point.y + k1.y * (tau / 2),
                point.z + k1.z * (tau / 2)
        };
        Point k2 = countDerivatives(send);
        send.x = point.x + k2.x * (tau / 2);
        send.y = point.y + k2.y * (tau / 2);
        send.z = point.z + k2.z * (tau / 2);
        Point k3 = countDerivatives(send);
        send.x = point.x + k3.x * tau;
        send.y = point.y + k3.y * tau;
        send.z = point.z + k3.z * tau;
        Point k4 = countDerivatives(send);
        return Point{
                point.x + (tau / 6) * (k1.x + k4.x + 2 * (k2.x + k3.x)),
                point.y + (tau / 6) * (k1.y + k4.y + 2 * (k2.y + k3.y)),
                point.z + (tau / 6) * (k1.z + k4.z + 2 * (k2.z + k3.z))
        };
    };
    generatePointsMainloop(std::forward<LambdaNewPointAction>(newPointAction),
                           point,
                           pointsCount,
                           std::move(nextPoint));
}

} // namespace Impl

template<typename LambdaDerivatives, typename LambdaNewPointAction>
void generatePoints(LambdaNewPointAction &&newPointAction,
                    Point point,
                    int pointsCount,
                    long double tau,
                    LambdaDerivatives &&countDerivatives) {
    Impl::setNextPointGenerator(std::forward<LambdaNewPointAction>(newPointAction),
                                point,
                                pointsCount,
                                tau,
                                std::forward<LambdaDerivatives>(countDerivatives));
}

}//namespace Model
