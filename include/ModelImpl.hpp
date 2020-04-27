#pragma once

#include <vector>
#include <cassert>

#include "Model.hpp"

namespace Model {

constexpr long double COORDINATE_VALUE_LIMIT = 1e3;

template<typename LambdaDerivatives, typename LambdaNewPointAction>
void generate_points_generate_points(LambdaNewPointAction new_point_action,
                                     Point point,
                                     int points_count,
                                     int steps_per_point,
                                     LambdaDerivatives next_point) {
    Point previous_point = point;
    for (int i = 0; i < points_count; ++i) {
        for (int j = 0; j < steps_per_point; ++j) {
            point = next_point(point);
        }
        if ((point.x == previous_point.x && point.y == previous_point.y && point.z == previous_point.z) ||
            point.x > COORDINATE_VALUE_LIMIT || point.y > COORDINATE_VALUE_LIMIT || point.z > COORDINATE_VALUE_LIMIT) {
            break;
        }
        previous_point = point;
        new_point_action(point);
    }
}


template<typename LambdaDerivatives, typename LambdaNewPointAction>
void generate_points_generate_next_point_func(LambdaNewPointAction new_point_action,
                                              Point point,
                                              int points_count,
                                              int steps_per_point,
                                              long double tau,
                                              LambdaDerivatives count_derivatives) {
    auto next_point = [tau, count_derivatives](const Point &point) {
        Point k1 = count_derivatives(point);
        Point send = {
                point.x + k1.x * tau / 2,
                point.y + k1.y * tau / 2,
                point.z + k1.z * tau / 2
        };
        Point k2 = count_derivatives(send);
        send.x = point.x + k2.x * tau / 2;
        send.y = point.y + k2.y * tau / 2;
        send.z = point.z + k2.z * tau / 2;
        Point k3 = count_derivatives(send);
        send.x = point.x + k3.x * tau;
        send.y = point.y + k3.y * tau;
        send.z = point.z + k3.z * tau;
        Point k4 = count_derivatives(send);
        return Point{
                point.x + (tau / 6) * (k1.x + k4.x + 2 * (k2.x + k3.x)),
                point.y + (tau / 6) * (k1.y + k4.y + 2 * (k2.y + k3.y)),
                point.z + (tau / 6) * (k1.z + k4.z + 2 * (k2.z + k3.z))
        };
    };
    generate_points_generate_points(new_point_action, point, points_count, steps_per_point, next_point);
}


template<typename LambdaDerivatives, typename LambdaNewPointAction>
void generate_points(LambdaNewPointAction new_point_action,
                     Point point,
                     int points_count,
                     int steps_per_point,
                     long double tau,
                     LambdaDerivatives count_derivatives) {
    generate_points_generate_next_point_func(new_point_action,
                                             point,
                                             points_count,
                                             steps_per_point,
                                             tau,
                                             count_derivatives);
}

}//namespace Model
