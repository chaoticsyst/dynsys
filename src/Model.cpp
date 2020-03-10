#include <vector>
#include <cassert>

#include "Model.h"
#include "Preferences.h"

namespace Model {

template<typename Lambda>
void generate_points_generate_points(const std::function<void(const Point &)> &new_point_action,
                                     Point point,
                                     int points_count,
                                     int steps_per_point,
                                     Lambda next_point) {
    Point previous_point = point;
    for (int i = 0; i < points_count; ++i) {
        for (int j = 0; j < steps_per_point; ++j) {
            point = next_point(point);
        }
        if ((point.x == previous_point.x && point.y == previous_point.y && point.z == previous_point.z) ||
            point.x > Preferences::COORDINATE_VALUE_LIMIT || point.y > Preferences::COORDINATE_VALUE_LIMIT ||
            point.z > Preferences::COORDINATE_VALUE_LIMIT) {
            break;
        }
        previous_point = point;
        new_point_action(point);
    }
}

template<typename Lambda>
void generate_points_generate_next_point_func(const std::function<void(const Point &)> &new_point_action,
                                              Point point,
                                              int points_count,
                                              int steps_per_point,
                                              long double tau,
                                              ModelName,
                                              const std::vector<long double> &,
                                              Lambda count_derivatives) {
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

inline void generate_points_generate_count_derivatives_func(const std::function<void(const Point &)> &new_point_action,
                                                            Point point,
                                                            int points_count,
                                                            int steps_per_point,
                                                            long double tau,
                                                            ModelName modelName,
                                                            const std::vector<long double> &constants) {
    switch (modelName) {
        case ModelName::ROSSLER : {
            long double a = constants[0];
            long double b = constants[1];
            long double c = constants[2];
            auto count_derivatives = [a, b, c](const Point &values) {
                return Point{
                        -values.y - values.z,
                        values.x + a * values.y,
                        b + values.z * (values.x - c)
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::LORENZ : {
            long double sigma = constants[0];
            long double r = constants[1];
            long double b = constants[2];
            auto count_derivatives = [sigma, r, b](const Point &values) {
                return Point{
                        sigma * (values.y - values.x),
                        values.x * (r - values.z) - values.y,
                        values.x * values.y - b * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::GIN : {
            long double g = constants[0];
            long double m = constants[1];
            auto count_derivatives = [g, m](const Point &values) {
                return Point{
                        values.x * (m - values.z) + values.y,
                        -values.x,
                        g * (values.x * (values.x + std::abs(values.x)) / 2 - values.z)
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::CHUA : {//I don't find good constants
            long double alpha = 9;//constants[0];
            long double beta = 17.7;//constants[1];
            long double a = -8.0 / 7;//constants[2];
            long double b = -5.0 / 7;//constants[3];
            long double c = 4.6;//constants[4];
            long double d = 7.2;//constants[5];
            auto count_derivatives = [alpha, beta, a, b, c, d](const Point &values) {
                return Point{
                        alpha * (values.y - values.x -
                                 (c * values.x + ((b - c) * (std::abs(values.x + d) - std::abs(values.x - d)) +
                                                  (a - b) * (std::abs(values.x + 1) - std::abs(values.x - 1))) / 2)),
                        values.x - values.y + values.z,
                        -beta * values.y
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::HR3 : {//periodic
            long double a = 1;//constants[0];
            long double b = 3;// constants[1];
            long double I = 2.7;//constants[2];
            long double c = 1;// constants[3];
            long double d = 5;//constants[4];
            long double r = 0.003;// constants[5];
            long double s = 4;//constants[6];
            long double alpha = -1.6;// constants[7];
            auto count_derivatives = [a, b, I, c, d, r, s, alpha](const Point &values) {
                return Point{
                        values.y - a * values.x * values.x * values.x + b * values.x * values.x - values.z + I,
                        c - d * values.x * values.x - values.y,
                        r * (s * (values.x - alpha) - values.z)
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::HENON : {//don't work yet
            long double a = 1.4;//constants[0];
            long double b = 0.3;//constants[1];
            auto count_derivatives = [a, b](const Point &values) {
                return Point{
                        1 + values.y - a * values.x * values.x,
                        b * values.x,
                        0
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::AIZAWA : {//beautiful (but works strange) (don't works)
            long double a = 0.25;//constants[0];
            long double b = 0.95;// constants[1];
            long double c = 0.6;//constants[2];
            long double d = 3.5;// constants[3];
            long double e = -0.7;//constants[4];
            long double f = 0.1;// constants[5];
            auto count_derivatives = [a, b, c, d, e, f](const Point &values) {
                return Point{
                        values.x * (values.z - b) - d * values.y,
                        d * values.x + values.y * (values.z - b),
                        c + a * values.z - values.z * values.z * values.z / 3 -
                        (values.x * values.x + values.y * values.y) * (1 + e * values.z) +
                        f * values.z * values.x * values.x * values.x
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::CHEN_LEE : {
            long double a = 5;//constants[0];
            long double b = -10;// constants[1];
            long double c = -0.38;//constants[2];
            auto count_derivatives = [a, b, c](const Point &values) {
                return Point{
                        a * values.x - values.y * values.z,
                        b * values.y + values.x * values.z,
                        c * values.z + values.x * values.y / 3

                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::ANISHCHENKO_ASTAKHOV: {
            long double a = 1.2;// constants[0];
            long double b = 0.5;// constants[1];
            auto count_derivatives = [a, b](const Point &values) {
                return Point{
                        a * values.x + values.y - values.x * values.z,
                        -values.x,
                        b * (-values.z + (values.x > 0 ? values.x * values.x : 0))
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::ARNEODO: {//Special case of COULLET
            long double a = -5.5;//constants[0];
            long double b = 3.5;// constants[1];
            long double c = -1;//constants[2];
            auto count_derivatives = [a, b, c](const Point &values) {
                return Point{
                        values.y,
                        values.z,
                        values.x * (c * values.x * values.x - a) - b * values.y - values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::BOUALI_2: {
            long double a = 0.3;// constants[0];
            long double b = 1;// constants[1];
            auto count_derivatives = [a, b](const Point &values) {
                return Point{
                        values.x * (4 - values.y) + a * values.z,
                        -values.y * (1 - values.x * values.x),
                        -values.x * (1.5 - b * values.z) - 0.05 * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::BURKE_SHAW: {
            long double a = 10;//constants[0];
            long double b = 4.272;// constants[1];
            auto count_derivatives = [a, b](const Point &values) {
                return Point{
                        -a * (values.x + values.y),
                        -values.y - a * values.x * values.z,
                        a * values.x * values.y + b
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::CHEN_CELIKOVSKY: {
            long double a = 36;//constants[0];
            long double b = 3;// constants[1];
            long double c = 20;//constants[2];
            auto count_derivatives = [a, b, c](const Point &values) {
                return Point{
                        a * (values.y - values.x),
                        -values.x * values.z + c * values.y,
                        values.x * values.y - b * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::COULLET: {
            long double a = 0.8;//constants[0];
            long double b = -1.1;// constants[1];
            long double c = -0.45;//constants[2];
            long double d = -1;// constants[3];
            auto count_derivatives = [a, b, c, d](const Point &values) {
                return Point{
                        values.y,
                        values.z,
                        values.x * (d * values.x * values.x + a) + b * values.y + c * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::DADRAS: {
            long double a = 3;//constants[0];
            long double b = 2.7;// constants[1];
            long double c = 1.7;//constants[2];
            long double d = 2;// constants[3];
            long double e = 9;//constants[4];
            auto count_derivatives = [a, b, c, d, e](const Point &values) {
                return Point{
                        values.y * (1 + b * values.z) - a * values.x,
                        c * values.y + values.z * (1 - values.x),
                        d * values.x * values.y - e * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::DEQUAN_LI: {
            long double a = 40;//constants[0];
            long double b = 1.833;// constants[1];
            long double c = 0.16;//constants[2];
            long double d = 0.65;// constants[3];
            long double e = 55;//constants[4];
            long double f = 20;// constants[5];
            auto count_derivatives = [a, b, c, d, e, f](const Point &values) {
                return Point{
                        a * (values.y - values.x) + c * values.x * values.z,
                        values.x * (e - values.z) + f * values.y,
                        b * values.z + values.x * (values.y - d * values.x)
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::FINANCE: {
            long double a_ = 0.001;//constants[0];
            long double b = 0.2;// constants[1];
            long double c = 1.1;//constants[2];
            long double a = 1 / b - a_;
            auto count_derivatives = [a, b, c](const Point &values) {
                return Point{
                        values.x * (a + values.y) + values.z,
                        -b * values.y - values.x * values.x,
                        -values.x - c * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::FOUR_WING: {
            long double a = 4;//constants[0];
            long double b = 6;// constants[1];
            long double c = 10;//constants[2];
            long double d = 5;// constants[3];
            long double e = 1;//constants[4];
            auto count_derivatives = [a, b, c, d, e](const Point &values) {
                return Point{
                        a * values.x - b * values.y * values.z,
                        c * values.y + values.x * values.z,
                        values.x * (e + values.z) - d * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::GENESIO_TESI: {
            long double a = 0.44;//constants[0];
            long double b = 1.1;// constants[1];
            long double c = 1;//constants[2];
            auto count_derivatives = [a, b, c](const Point &values) {
                return Point{
                        values.y,
                        values.z,
                        values.x * (values.x - c) - a * values.z - b * values.y
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::HADLEY: {
            long double a = 0.2;//constants[0];
            long double b = 4;// constants[1];
            long double c_ = 8;//constants[2];
            long double d = 1;// constants[3];
            long double c = a * c_;
            auto count_derivatives = [a, b, c, d](const Point &values) {
                return Point{
                        -values.y * values.y - values.z * values.z - a * values.x + c,
                        values.x * (values.y - b * values.z) - values.y + d,
                        values.x * (b * values.y + values.z) - values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::HALVORSEN: {
            long double a = 1.4;//constants[0];
            auto count_derivatives = [a](const Point &values) {
                return Point{
                        -a * values.x - 4 * (values.y + values.z) - values.y * values.y,
                        -a * values.y - 4 * (values.z + values.x) - values.z * values.z,
                        -a * values.z - 4 * (values.x + values.y) - values.x * values.x
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::LIU_CHEN: {
            long double a = 2.4;//constants[0];
            long double b = -3.78;// constants[1];
            long double c = 14;//constants[2];
            long double d = -11;// constants[3];
            long double e = 4;//constants[4];
            long double f = 5.58;// constants[5];
            long double g = 1;// constants[6];
            auto count_derivatives = [a, b, c, d, e, f, g](const Point &values) {
                return Point{
                        values.y * (a + c * values.z) + b * values.x,
                        d * values.y + values.z * (e * values.x - 1),
                        f * values.z + g * values.x * values.y
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::LORENZ_MOD_1: {
            long double a = 0.1;//constants[0];
            long double b = 4;// constants[1];
            long double c_ = 14;//constants[2];
            long double d = 0.08;// constants[3];
            long double c = a * c_;
            auto count_derivatives = [a, b, c, d](const Point &values) {
                return Point{
                        -a * values.x + values.y * values.y - values.z * values.z + c,
                        values.x * (values.y - b * values.z) + d,
                        values.z + values.x * (b * values.y + values.z)
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::LORENZ_MOD_2: {//Looks like mod1 with
            long double a = 0.9;//constants[0];
            long double b = 5;// constants[1];
            long double c_ = 9.9;//constants[2];
            long double d = 1;// constants[3];
            long double c = a * c_;
            auto count_derivatives = [a, b, c, d](const Point &values) {
                return Point{
                        -a * values.x + values.y * values.y - values.z * values.z + c,
                        values.x * (values.y - b * values.z) + d,
                        -values.z + values.x * (b * values.y + values.z)
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::LU_CHEN: {
            long double a = -10;//constants[0];
            long double b = -4;// constants[1];
            long double c = 18.1;//constants[2];
            long double d = -(a * b) / (a + b);
            auto count_derivatives = [a, b, c, d](const Point &values) {
                return Point{
                        d * values.x - values.y * values.z + c,
                        a * values.y + values.x * values.z,
                        b * values.z + values.x * values.y
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::NEWTON_LEIPNIK: {
            long double a = 0.4;//constants[0];
            long double b = 0.175;// constants[1];
            auto count_derivatives = [a, b](const Point &values) {
                return Point{
                        -a * values.x + values.y * (1 + 10 * values.z),
                        values.x * (5 * values.z - 1) - 0.4 * values.y,
                        b * values.z - 5 * values.x * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::NOSE_HOOVER: {
            long double a = 1.5;//constants[0];
            auto count_derivatives = [a](const Point &values) {
                return Point{
                        values.y,
                        values.y * values.z - values.x,
                        a - values.y * values.y
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::QI_CHEN: {
            long double a = 38;//constants[0];
            long double b = 8.0 / 3;// constants[1];
            long double c = 80;//constants[2];
            auto count_derivatives = [a, b, c](const Point &values) {
                return Point{
                        a * (values.y - values.x) + values.y * values.z,
                        values.x * (c - values.z) + values.y,
                        values.x * values.y - b * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::RAYLEIGH_BENARD: {
            long double a = 9;//constants[0];
            long double b = 12;// constants[1];
            long double c = 5;//constants[2];
            auto count_derivatives = [a, b, c](const Point &values) {
                return Point{
                        a * (values.y - values.x),
                        values.x * (b - values.z) - values.y,
                        values.x * values.y - c * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::RUCKLIDGE: {
            long double a = 2;//constants[0];
            long double b = 6.7;// constants[1];
            auto count_derivatives = [a, b](const Point &values) {
                return Point{
                        values.y * (b - values.z) - a * values.x,
                        values.x,
                        values.y * values.y - values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::SAKARAYA: {
            long double a = 0.4;//constants[0];
            long double b = 0.3;// constants[1];
            auto count_derivatives = [a, b](const Point &values) {
                return Point{
                        values.y * (values.z + 1) - values.x,
                        values.x * (a * values.z - 1) - values.y,
                        values.z - b * values.x * values.y
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::SHIMIZU_MORIOKA: {
            long double a = 0.75;//constants[0];
            long double b = 0.45;// constants[1];
            auto count_derivatives = [a, b](const Point &values) {
                return Point{
                        values.y,
                        values.x * (1 - values.z) - a * values.y,
                        values.x * values.x - b * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::THOMAS: {
            long double a = 0.19;//constants[0];
            auto count_derivatives = [a](const Point &values) {
                return Point{
                        a * values.x + std::sin(values.y),
                        -a * values.y + std::sin(values.z),
                        -a * values.z + std::sin(values.x),
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::TSUCS1: {
            long double a = 40;//constants[0];
            long double b = 0.833;// constants[1];
            long double c = 0.5;//constants[2];
            long double d = 0.65;// constants[3];
            long double e = 20;//constants[4];
            auto count_derivatives = [a, b, c, d, e](const Point &values) {
                return Point{
                        a * (values.y - values.x) + c * values.x * values.z,
                        e * values.y - values.x * values.z,
                        b * values.z + values.x * (values.y - d * values.x)
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::TSUCS2: {
            long double a = 40;//constants[0];
            long double b = 55;// constants[1];
            long double c = 1.833;//constants[2];
            long double d = 0.16;// constants[3];
            long double e = 0.65;//constants[4];
            long double f = 20;// constants[5];
            auto count_derivatives = [a, b, c, d, e, f](const Point &values) {
                return Point{
                        a * (values.y - values.x) + d * values.x * values.z,
                        f * values.y + values.x * (b - values.z),
                        c * values.z + values.x * (values.y - e * values.x)
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::WANG_SUN: {
            long double a = 0.2;//constants[0];
            long double b = -0.01;// constants[1];
            long double c = 1;//constants[2];
            long double d = -0.4;// constants[3];
            long double e = -1;//constants[4];
            long double f = -1;// constants[5];
            auto count_derivatives = [a, b, c, d, e, f](const Point &values) {
                return Point{
                        a * values.x + c * values.y * values.z,
                        values.x * (b - values.z) + d * values.y,
                        e * values.z + f * values.x * values.y
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::WIMOL_BANLUE: {
            long double a = 2;//constants[0];
            auto count_derivatives = [a](const Point &values) {
                return Point{
                        values.y - values.x,
                        -values.z * std::tanh(values.x),
                        values.x * values.y + std::abs(values.y) - a
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        case ModelName::YU_WANG: {
            long double a = 10;//constants[0];
            long double b = 40;// constants[1];
            long double c = 2;//constants[2];
            long double d = 2.5;// constants[3];
            auto count_derivatives = [a, b, c, d](const Point &values) {
                return Point{
                        a * (values.y - values.x),
                        values.x * (b - c * values.z),
                        std::exp(values.x * values.y) - d * values.z
                };
            };
            generate_points_generate_next_point_func(new_point_action,
                                                     point,
                                                     points_count,
                                                     steps_per_point,
                                                     tau,
                                                     modelName,
                                                     constants,
                                                     count_derivatives);
        }
            break;
        default:
            assert(false); //unimplemented models
    }
}

void generate_points(const std::function<void(const Point &)> &new_point_action,
                     Point point,
                     int points_count,
                     int steps_per_point,
                     long double tau,
                     ModelName modelName,
                     const std::vector<long double> &constants) {
    generate_points_generate_count_derivatives_func(new_point_action,
                                                    point,
                                                    points_count,
                                                    steps_per_point,
                                                    tau,
                                                    modelName,
                                                    constants);
}

}//namespace Model
