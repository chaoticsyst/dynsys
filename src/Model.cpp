#include <vector>
#include "Model.h"

static long double a, b, c, tau;

void set_constants(long double a_, long double b_, long double c_) {
    a = a_;
    b = b_;
    c = c_;
}

void set_constant_a(long double a_) {
    a = a_;
}

void set_constant_b(long double b_) {
    b = b_;
}

void set_constant_c(long double c_) {
    c = c_;
}

//Формулы по уравнениям для аттрактора Рёсслера
static Point count_derivatives(const Point &values) {
    return Point{
            -values.y - values.z,
            values.x + a * values.y,
            b + values.z * (values.x - c)
    };
}

//Самый простой метод Рунге — Кутты
static Point next_point(const Point &point) {
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
}

std::vector<Point> generate_points(Point point, int points_count, int steps_per_point, long double tau_) {
    tau = tau_;
    std::vector<Point> time_line(points_count);
    for (int i = 0; i < points_count; ++i) {
        for (int j = 0; j < steps_per_point; ++j) {
            point = next_point(point);
        }
        time_line[i] = point;
    }
    return std::move(time_line);
}
