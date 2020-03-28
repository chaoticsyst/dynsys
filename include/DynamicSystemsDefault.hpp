#pragma once

#include <cmath>

#include "Model.h"

namespace Model {

inline auto get_derivatives_function_lorenz(long double sigma, long double r, long double b) {
    return [sigma, r, b](const Point &values) {
        return Point{
                sigma * (values.y - values.x),
                values.x * (r - values.z) - values.y,
                values.x * values.y - b * values.z
        };
    };
}


inline auto get_derivatives_function_rossler(long double a, long double b, long double c) {
    return [a, b, c](const Point &values) {
        return Point{
                -values.y - values.z,
                values.x + a * values.y,
                b + values.z * (values.x - c)
        };
    };
}


inline auto get_derivatives_function_gin(long double g, long double m) {
    return [g, m](const Point &values) {
        return Point{
                values.x * (m - values.z) + values.y,
                -values.x,
                g * (values.x * (values.x + std::abs(values.x)) / 2 - values.z)
        };
    };
}


inline auto get_derivatives_function_chua(long double alpha, long double beta, long double a, long double b, long double c, long double d) {
    return [alpha, beta, a, b, c, d](const Point &values) {
        return Point{
                alpha * (values.y - values.x -
                         (c * values.x + ((b - c) * (std::abs(values.x + d) - std::abs(values.x - d)) +
                                          (a - b) * (std::abs(values.x + 1) - std::abs(values.x - 1))) / 2)),
                values.x - values.y + values.z,
                -beta * values.y
        };
    };
}


inline auto get_derivatives_function_hr3(long double a, long double b, long double c, long double d, long double r, long double s, long double alpha,
                                         long double I) {
    return [a, b, I, c, d, r, s, alpha](const Point &values) {
        return Point{
                values.y - a * values.x * values.x * values.x + b * values.x * values.x - values.z + I,
                c - d * values.x * values.x - values.y,
                r * (s * (values.x - alpha) - values.z)
        };
    };
}


inline auto get_derivatives_function_henon(long double a, long double b) {
    return [a, b](const Point &values) {
        return Point{
                1 + values.y - a * values.x * values.x,
                b * values.x,
                0
        };
    };
}


inline auto get_derivatives_function_aizawa(long double a, long double b, long double c, long double d, long double e, long double f) {
    return [a, b, c, d, e, f](const Point &values) {
        return Point{
                values.x * (values.z - b) - d * values.y,
                d * values.x + values.y * (values.z - b),
                c + a * values.z - values.z * values.z * values.z / 3 -
                (values.x * values.x + values.y * values.y) * (1 + e * values.z) +
                f * values.z * values.x * values.x * values.x
        };
    };
}


inline auto get_derivatives_function_chen_lee(long double a, long double b, long double c) {
    return [a, b, c](const Point &values) {
        return Point{
                a * values.x - values.y * values.z,
                b * values.y + values.x * values.z,
                c * values.z + values.x * values.y / 3

        };
    };
}


inline auto get_derivatives_function_anishchenko_astakhov(long double a, long double b) {
    return [a, b](const Point &values) {
        return Point{
                a * values.x + values.y - values.x * values.z,
                -values.x,
                b * (-values.z + (values.x > 0 ? values.x * values.x : 0))
        };
    };
}


inline auto get_derivatives_function_bouali_2(long double a, long double b) {
    return [a, b](const Point &values) {
        return Point{
                values.x * (4 - values.y) + a * values.z,
                -values.y * (1 - values.x * values.x),
                -values.x * (1.5 - b * values.z) - 0.05 * values.z
        };
    };
}


inline auto get_derivatives_function_burke_shaw(long double a, long double b) {
    return [a, b](const Point &values) {
        return Point{
                -a * (values.x + values.y),
                -values.y - a * values.x * values.z,
                a * values.x * values.y + b
        };
    };
}


inline auto get_derivatives_function_chen_celikovsky(long double a, long double b, long double c) {
    return [a, b, c](const Point &values) {
        return Point{
                a * (values.y - values.x),
                -values.x * values.z + c * values.y,
                values.x * values.y - b * values.z
        };
    };
}


inline auto get_derivatives_function_coullet(long double a, long double b, long double c, long double d) {
    return [a, b, c, d](const Point &values) {
        return Point{
                values.y,
                values.z,
                values.x * (d * values.x * values.x + a) + b * values.y + c * values.z
        };
    };
}


inline auto get_derivatives_function_dadras(long double a, long double b, long double c, long double d, long double e) {
    return [a, b, c, d, e](const Point &values) {
        return Point{
                values.y * (1 + b * values.z) - a * values.x,
                c * values.y + values.z * (1 - values.x),
                d * values.x * values.y - e * values.z
        };
    };
}


inline auto get_derivatives_function_dequan_li(long double a, long double b, long double c, long double d, long double e, long double f) {
    return [a, b, c, d, e, f](const Point &values) {
        return Point{
                a * (values.y - values.x) + c * values.x * values.z,
                values.x * (e - values.z) + f * values.y,
                b * values.z + values.x * (values.y - d * values.x)
        };
    };
}


inline auto get_derivatives_function_finance(long double a, long double b, long double c) {
    long double a_ = 1 / b - a;
    return [a_, b, c](const Point &values) {
        return Point{
                values.x * (a_ + values.y) + values.z,
                -b * values.y - values.x * values.x,
                -values.x - c * values.z
        };
    };
}


inline auto get_derivatives_function_four_wing(long double a, long double b, long double c, long double d, long double e) {
    return [a, b, c, d, e](const Point &values) {
        return Point{
                a * values.x - b * values.y * values.z,
                c * values.y + values.x * values.z,
                values.x * (e + values.z) - d * values.z
        };
    };
}


inline auto get_derivatives_function_genesio_tesi(long double a, long double b, long double c) {
    return [a, b, c](const Point &values) {
        return Point{
                values.y,
                values.z,
                values.x * (values.x - c) - a * values.z - b * values.y
        };
    };
}


inline auto get_derivatives_function_hadley(long double a, long double b, long double c, long double d) {
    long double c_ = a * c;
    return [a, b, c_, d](const Point &values) {
        return Point{
                -values.y * values.y - values.z * values.z - a * values.x + c_,
                values.x * (values.y - b * values.z) - values.y + d,
                values.x * (b * values.y + values.z) - values.z
        };
    };
}


inline auto get_derivatives_function_halvorsen(long double a) {
    return [a](const Point &values) {
        return Point{
                -a * values.x - 4 * (values.y + values.z) - values.y * values.y,
                -a * values.y - 4 * (values.z + values.x) - values.z * values.z,
                -a * values.z - 4 * (values.x + values.y) - values.x * values.x
        };
    };
}


inline auto
get_derivatives_function_liu_chen(long double a, long double b, long double c, long double d, long double e, long double f, long double g) {
    return [a, b, c, d, e, f, g](const Point &values) {
        return Point{
                values.y * (a + c * values.z) + b * values.x,
                d * values.y + values.z * (e * values.x - 1),
                f * values.z + g * values.x * values.y
        };
    };
}


inline auto get_derivatives_function_lorenz_mod_1(long double a, long double b, long double c, long double d) {
    long double c_ = a * c;
    return [a, b, c_, d](const Point &values) {
        return Point{
                -a * values.x + values.y * values.y - values.z * values.z + c_,
                values.x * (values.y - b * values.z) + d,
                values.z + values.x * (b * values.y + values.z)
        };
    };
}


inline auto get_derivatives_function_lorenz_mod_2(long double a, long double b, long double c, long double d) {
    long double c_ = a * c;
    return [a, b, c_, d](const Point &values) {
        return Point{
                -a * values.x + values.y * values.y - values.z * values.z + c_,
                values.x * (values.y - b * values.z) + d,
                -values.z + values.x * (b * values.y + values.z)
        };
    };
}


inline auto get_derivatives_function_lu_chen(long double a, long double b, long double c) {
    long double d = -(a * b) / (a + b);
    return [a, b, c, d](const Point &values) {
        return Point{
                d * values.x - values.y * values.z + c,
                a * values.y + values.x * values.z,
                b * values.z + values.x * values.y
        };
    };
}


inline auto get_derivatives_function_newton_leipnik(long double a, long double b) {
    return [a, b](const Point &values) {
        return Point{
                -a * values.x + values.y * (1 + 10 * values.z),
                values.x * (5 * values.z - 1) - 0.4 * values.y,
                b * values.z - 5 * values.x * values.z
        };
    };
}


inline auto get_derivatives_function_nose_hoover(long double a) {
    return [a](const Point &values) {
        return Point{
                values.y,
                values.y * values.z - values.x,
                a - values.y * values.y
        };
    };
}


inline auto get_derivatives_function_qi_chen(long double a, long double b, long double c) {
    return [a, b, c](const Point &values) {
        return Point{
                a * (values.y - values.x) + values.y * values.z,
                values.x * (c - values.z) + values.y,
                values.x * values.y - b * values.z
        };
    };
}


inline auto get_derivatives_function_rayleigh_benard(long double a, long double b, long double c) {
    return [a, b, c](const Point &values) {
        return Point{
                a * (values.y - values.x),
                values.x * (b - values.z) - values.y,
                values.x * values.y - c * values.z
        };
    };
}


inline auto get_derivatives_function_rucklidge(long double a, long double b) {
    return [a, b](const Point &values) {
        return Point{
                values.y * (b - values.z) - a * values.x,
                values.x,
                values.y * values.y - values.z
        };
    };
}


inline auto get_derivatives_function_sakaraya(long double a, long double b) {
    return [a, b](const Point &values) {
        return Point{
                values.y * (values.z + 1) - values.x,
                values.x * (a * values.z - 1) - values.y,
                values.z - b * values.x * values.y
        };
    };
}


inline auto get_derivatives_function_shimizu_morioka(long double a, long double b) {
    return [a, b](const Point &values) {
        return Point{
                values.y,
                values.x * (1 - values.z) - a * values.y,
                values.x * values.x - b * values.z
        };
    };
}


inline auto get_derivatives_function_thomas(long double a) {
    return [a](const Point &values) {
        return Point{
                a * values.x + std::sin(values.y),
                -a * values.y + std::sin(values.z),
                -a * values.z + std::sin(values.x),
        };
    };
}


inline auto get_derivatives_function_tsucs1(long double a, long double b, long double c, long double d, long double e) {
    return [a, b, c, d, e](const Point &values) {
        return Point{
                a * (values.y - values.x) + c * values.x * values.z,
                e * values.y - values.x * values.z,
                b * values.z + values.x * (values.y - d * values.x)
        };
    };
}


inline auto get_derivatives_function_tsucs2(long double a, long double b, long double c, long double d, long double e, long double f) {
    return [a, b, c, d, e, f](const Point &values) {
        return Point{
                a * (values.y - values.x) + d * values.x * values.z,
                f * values.y + values.x * (b - values.z),
                c * values.z + values.x * (values.y - e * values.x)
        };
    };
}


inline auto get_derivatives_function_wang_sun(long double a, long double b, long double c, long double d, long double e, long double f) {
    return [a, b, c, d, e, f](const Point &values) {
        return Point{
                a * values.x + c * values.y * values.z,
                values.x * (b - values.z) + d * values.y,
                e * values.z + f * values.x * values.y
        };
    };
}


inline auto get_derivatives_function_wimol_banlue(long double a) {
    return [a](const Point &values) {
        return Point{
                values.y - values.x,
                -values.z * std::tanh(values.x),
                values.x * values.y + std::abs(values.y) - a
        };
    };
}


inline auto get_derivatives_function_yu_wang(long double a, long double b, long double c, long double d) {
    return [a, b, c, d](const Point &values) {
        return Point{
                a * (values.y - values.x),
                values.x * (b - c * values.z),
                std::exp(values.x * values.y) - d * values.z
        };
    };
}


} //namespace Model