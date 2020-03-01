#include <vector>
#include "Model.h"

constexpr long double COORDINATE_VALUE_LIMIT = 1000; //generation stops if the coordinate value becomes greater

namespace Model {

inline std::function<Point(Point)> generate_count_derivatives_function(const std::vector<long double> &constants,
                                                                       ModelName modelName) {
    switch (modelName) {
        case ModelName::Rossler : {
            long double a = constants[0];
            long double b = constants[1];
            long double c = constants[2];
            return [a, b, c](const Point &values) {
                return Point{
                        -values.y - values.z,
                        values.x + a * values.y,
                        b + values.z * (values.x - c)
                };
            };
        }
        case ModelName::Lorenz : {
            long double sigma = constants[0];
            long double r = constants[1];
            long double b = constants[2];
            return [sigma, r, b](const Point &values) {
                return Point{
                        sigma * (values.y - values.x),
                        values.x * (r - values.z) - values.y,
                        values.x * values.y - b * values.z
                };
            };
        }
        default:
            std::abort();
    }
}

inline auto generate_next_point_function(long double tau,
                                         const std::vector<long double> &constants,
                                         ModelName modelName) {
    auto count_derivatives = generate_count_derivatives_function(constants, modelName);
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
    return next_point;
}

void generate_points(const std::function<void(const Point &)> &new_point_action,
                     Point point,
                     int points_count,
                     int steps_per_point,
                     long double tau,
                     ModelName modelName,
                     const std::vector<long double> &constants) {
    Point previous_point = point;
    auto next_point = generate_next_point_function(tau, constants, modelName);
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

}//namespace Model