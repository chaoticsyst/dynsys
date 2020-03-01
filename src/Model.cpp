#include <vector>
#include "Model.h"

namespace Model {

auto generate_next_point_function(long double tau, const std::vector<long double> &constants) {
    long double a = constants[0];
    long double b = constants[1];
    long double c = constants[2];
    //Уравнения для аттрактора Рёсслера
    auto count_derivatives = [a, b, c](const Point &values) {
        return Point{
                -values.y - values.z,
                values.x + a * values.y,
                b + values.z * (values.x - c)
        };
    };
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
                     const std::vector<long double> &constants) {
    auto next_point = generate_next_point_function(tau, constants);
    for (int i = 0; i < points_count; ++i) {
        for (int j = 0; j < steps_per_point; ++j) {
            point = next_point(point);
        }
        new_point_action(point);
    }
}

}//namespace Model