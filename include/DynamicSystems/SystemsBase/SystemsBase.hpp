#pragma once

#include <vector>
#include <array>
#include <string>
#include <cmath>

#include "DynamicSystems/DynamicSystem.hpp"


namespace DynamicSystems {
namespace AllSystems {

template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemLorenz() {
    std::string attractorName = "The Lorenz attractor";
    std::array<std::string, 3> formulae = {"a*(y - x)",
                                           "x*(r - z) - y",
                                           "x*y - b*z"};
    std::vector<std::string> constantsNames = {"a", "r", "b"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {10, 28,  8.0 / 3.0}},
            {"Cycle",          {10, 100, 8.0 / 3.0}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [sigma = constValues[0], r = constValues[1], b = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    sigma * (values.y - values.x),
                    values.x * (r - values.z) - values.y,
                    values.x * values.y - b * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemRossler() {
    std::string attractorName = "The Rossler attractor";
    std::array<std::string, 3> formulae = {"-z - x",
                                           "x + a*y",
                                           "b + z*(x - c)"};
    std::vector<std::string> constantsNames = {"a", "b", "c"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values",   {0.2, 0.2, 5.7}},
            {"Cycle",            {0.2, 0.2, 3}},
            {"Classic values 2", {0.1, 0.1, 14}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    -values.y - values.z,
                    values.x + a * values.y,
                    b + values.z * (values.x - c)
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemChua() {
    std::string attractorName = "The Chua attractor";
    std::array<std::string, 3> formulae = {
            "s*(y - x - (c*x + 0.5*(b - c)(|x + d| - |x - d|) + 0.5*(a - b)(|x + 1| - |x - 1|)))",
            "x - y + z",
            "-r*y"};
    std::vector<std::string> constantsNames = {"s", "r", "a", "b", "c", "d"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values",   {11,   14,    -0.713, -0.455, 4.6, 7.2}},
            {"Classic values 2", {15.6, 25.58, -2,     0,      0,   0}},
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [s = constValues[0], r = constValues[1],
                a = (constValues[2] - constValues[3]) / 2,
                b = (constValues[3] - constValues[4]) / 2,
                c = 1 + constValues[4], d = constValues[5]](const Model::Point &values) {
            return Model::Point{
                    s * (values.y - c * values.x
                         - (b * (std::abs(values.x + d) - std::abs(values.x - d)) +
                            a * (std::abs(values.x + 1) - std::abs(values.x - 1)))),
                    values.x - values.y + values.z,
                    -r * values.y
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemHR3() {
    std::string attractorName = "The Hindmarsh-Rose system";
    std::array<std::string, 3> formulae = {"y - a*x^3 + b*x^2 - z + I",
                                           "c - d*x^2 - y",
                                           "r*(s*(x - alpha) - z)"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {1, 3, 2.7, 1, 5, 0.003, 4, -1.6}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], I = constValues[2], c = constValues[3], d = constValues[4],
                r = constValues[5], s = constValues[6], alpha = constValues[7]](const Model::Point &values) {
            return Model::Point{
                    values.y - a * values.x * values.x * values.x + b * values.x * values.x - values.z + I,
                    c - d * values.x * values.x - values.y,
                    r * (s * (values.x - alpha) - values.z)
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemAizawa() {
    std::string attractorName = "The Aizawa attractor";
    std::array<std::string, 3> formulae = {"(z - b)*x - d*y",
                                           "d*x + (z - b)*y",
                                           "c + a*z - (z^3 / 3) - (x^2 + y^2)*(1+f*z) + g*z*x^3"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d", "f", "g"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.95, 0.7, 0.6, 3.5, 0.25, 0.1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2], d = constValues[3],
                e = constValues[4], f = constValues[5]](const Model::Point &values) {
            return Model::Point{
                    values.x * (values.z - b) - d * values.y,
                    d * values.x + values.y * (values.z - b),
                    c + a * values.z - values.z * values.z * values.z / 3 -
                    (values.x * values.x + values.y * values.y) * (1 + e * values.z) +
                    f * values.z * values.x * values.x * values.x
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemChenLee() {
    std::string attractorName = "The Chen-Lee attractor";
    std::array<std::string, 3> formulae = {"a*x - y*z",
                                           "b*y + x*z",
                                           "c*z + x*y/3"};
    std::vector<std::string> constantsNames = {"a", "b", "c"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {5, -10, -0.38}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    a * values.x - values.y * values.z,
                    b * values.y + values.x * values.z,
                    c * values.z + values.x * values.y / 3

            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemAnishenkoAstakhov() {
    std::string attractorName = "The Anishenko-Astakhov attractor";
    std::array<std::string, 3> formulae = {"x*(a - z) + y",
                                           "-x",
                                           "b*(x*(x + |x|)/2 - z)"};
    std::vector<std::string> constantsNames = {"a", "b"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {1.2, 0.5}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1]](const Model::Point &values) {
            return Model::Point{
                    values.x * (a - values.z) + values.y,
                    -values.x,
                    b * (-values.z + (values.x > 0 ? values.x * values.x : 0))
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemBouali2() {
    std::string attractorName = "The second Bouali attractor";
    std::array<std::string, 3> formulae = {"x*(4 - y) + a*z",
                                           "-y*(1 - x^2)",
                                           "-x*(1.5 - b*z) - 0.05*z"};
    std::vector<std::string> constantsNames = {"a", "b"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.3, 1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1]](const Model::Point &values) {
            return Model::Point{
                    values.x * (4 - values.y) + a * values.z,
                    -values.y * (1 - values.x * values.x),
                    -values.x * (1.5 - b * values.z) - 0.05 * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemBurkeShaw() {
    std::string attractorName = "The Burke-Shaw attractor";
    std::array<std::string, 3> formulae = {"-a*(x + y)",
                                           "-y - a*x*z",
                                           "a*x*z + b"};
    std::vector<std::string> constantsNames = {"a", "b"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {10, 4.272}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1]](const Model::Point &values) {
            return Model::Point{
                    -a * (values.x + values.y),
                    -values.y - a * values.x * values.z,
                    a * values.x * values.y + b
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemChenCelikovsky() {
    std::string attractorName = "The Chen-Celikovsky attractor";
    std::array<std::string, 3> formulae = {"a*(y - x)",
                                           "-x*z + c*y",
                                           "x*y - b*z"};
    std::vector<std::string> constantsNames = {"a", "b", "c"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {36, 3, 20}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    a * (values.y - values.x),
                    -values.x * values.z + c * values.y,
                    values.x * values.y - b * values.z
            };
        };
    };
    return {
            attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}
    };
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemCoullet() {
    std::string attractorName = "The Collet attractor";
    std::array<std::string, 3> formulae = {"y",
                                           "z",
                                           "a*x + b*y + c*z + d*x^3"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.8, -1.1, -0.45, -1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2],
                d = constValues[3]](const Model::Point &values) {
            return Model::Point{
                    values.y,
                    values.z,
                    values.x * (d * values.x * values.x + a) + b * values.y + c * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemDadras() {
    std::string attractorName = "The Dadras attractor";
    std::array<std::string, 3> formulae = {"y - a*x + b*y*z",
                                           "c*y - x*z + z",
                                           "d*x*y - f*z"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d", "f"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {3, 2.7, 1.7, 2, 9}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2], d = constValues[3],
                e = constValues[4]](const Model::Point &values) {
            return Model::Point{
                    values.y * (1 + b * values.z) - a * values.x,
                    c * values.y + values.z * (1 - values.x),
                    d * values.x * values.y - e * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemDequanLi() {
    std::string attractorName = "The Dequan Li attractor";
    std::array<std::string, 3> formulae = {"a*(y - z) + c*x*z",
                                           "f*x + g*y - x*z",
                                           "b*z + x*y - d*x^2"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d", "f", "g"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {40, 1.833, 0.16, 0.65, 55, 20}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2], d = constValues[3],
                e = constValues[4], f = constValues[5]](const Model::Point &values) {
            return Model::Point{
                    a * (values.y - values.x) + c * values.x * values.z,
                    values.x * (e - values.z) + f * values.y,
                    b * values.z + values.x * (values.y - d * values.x)
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemFinance() {
    std::string attractorName = "The Finance attractor";
    std::array<std::string, 3> formulae = {"(1/b - a)*x + z + x*y",
                                           "-b*y - x^2",
                                           "-x - c*z"};
    std::vector<std::string> constantsNames = {"a", "b", "c"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values",   {0.001, 0.2,  1.1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a_ = (1 / constValues[1] - constValues[0]), b = constValues[1],
                c = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    values.x * (a_ + values.y) + values.z,
                    -b * values.y - values.x * values.x,
                    -values.x - c * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemFourWing() {
    std::string attractorName = "The Four-Wing attractor";
    std::array<std::string, 3> formulae = {"a*x - b*y*z",
                                           "c*y + x*z",
                                           "f*x - d*z + x*z"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d", "f"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {4, 6, 10, 5, 1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2], d = constValues[3],
                e = constValues[4]](const Model::Point &values) {
            return Model::Point{
                    a * values.x - b * values.y * values.z,
                    c * values.y + values.x * values.z,
                    values.x * (e + values.z) - d * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemGenesioTesi() {
    std::string attractorName = "The Genesio-Tesi attractor";
    std::array<std::string, 3> formulae = {"y",
                                           "z",
                                           "a*x + b*y + c*z + x^2"};
    std::vector<std::string> constantsNames = {"a", "b", "c"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.44, 1.1, 1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    values.y,
                    values.z,
                    values.x * (values.x - c) - a * values.z - b * values.y
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemHadley() {
    std::string attractorName = "The Hadley attractor";
    std::array<std::string, 3> formulae = {"-y^2 - z^2 - a*x + a*c",
                                           "x*y - b*x*y - y + d",
                                           "b*x*y + x*z - z"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.2, 4, 8, 1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c_ = constValues[0] * constValues[2],
                d = constValues[3]](const Model::Point &values) {
            return Model::Point{
                    -values.y * values.y - values.z * values.z - a * values.x + c_,
                    values.x * (values.y - b * values.z) - values.y + d,
                    values.x * (b * values.y + values.z) - values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemHalvorsen() {
    std::string attractorName = "The Halvorsen attractor";
    std::array<std::string, 3> formulae = {"-a*x - 4*(y + z) - y^2",
                                           "-a*y - 4*(x + x) - z^2",
                                           "-a*z - 4*(x + y) - x^2"};
    std::vector<std::string> constantsNames = {"a"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {1.4}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0]](const Model::Point &values) {
            return Model::Point{
                    -a * values.x - 4 * (values.y + values.z) - values.y * values.y,
                    -a * values.y - 4 * (values.z + values.x) - values.z * values.z,
                    -a * values.z - 4 * (values.x + values.y) - values.x * values.x
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemLiuChen() {
    std::string attractorName = "The Liu-Chen attractor";
    std::array<std::string, 3> formulae = {"a*y + b*x + c*y*z",
                                           "d*y - z + f*x*z",
                                           "g*z + h*x*y"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d", "f", "g", "h"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {2.4, -3.78, 14, -11, 4, 5.58, 1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2], d = constValues[3],
                e = constValues[4], f = constValues[5], g = constValues[6]](const Model::Point &values) {
            return Model::Point{
                    values.y * (a + c * values.z) + b * values.x,
                    d * values.y + values.z * (e * values.x - 1),
                    f * values.z + g * values.x * values.y
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemLorenzMod1() {
    std::string attractorName = "The Lorenz Mod 1 attractor";
    std::array<std::string, 3> formulae = {"-a*x + y^2 - z^2  + a*c",
                                           "x*(y - b*z) + d",
                                           "z  + x*(b*y + z)"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.1, 4, 14, 0.08}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c_ = constValues[0] * constValues[2],
                d = constValues[3]](const Model::Point &values) {
            return Model::Point{
                    -a * values.x + values.y * values.y - values.z * values.z + c_,
                    values.x * (values.y - b * values.z) + d,
                    values.z + values.x * (b * values.y + values.z)
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemLorenzMod2() {
    std::string attractorName = "The Lorenz Mod 2 attractor";
    std::array<std::string, 3> formulae = {"-a*x + y^2 - z^2 + a*c",
                                           "x*(y - b*z) + d",
                                           "-z  + x*(b*y + z)"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.9, 5, 9.9, 1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c_ = constValues[0] * constValues[2],
                d = constValues[3]](const Model::Point &values) {
            return Model::Point{
                    -a * values.x + values.y * values.y - values.z * values.z + c_,
                    values.x * (values.y - b * values.z) + d,
                    -values.z + values.x * (b * values.y + values.z)
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemLuChen() {
    std::string attractorName = "The Lu-Chen attractor";
    std::array<std::string, 3> formulae = {"(-a*b*x)/(a+b) - y*z + c",
                                           "a*y + x*z",
                                           "b*z + x*y"};
    std::vector<std::string> constantsNames = {"a", "b", "c"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {-10, -4, 18.1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [d = constValues[0] * constValues[1] / (constValues[0] + constValues[1]),
                a = constValues[0], b = constValues[1], c = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    d * values.x - values.y * values.z + c,
                    a * values.y + values.x * values.z,
                    b * values.z + values.x * values.y
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemNewtonLeipnik() {
    std::string attractorName = "The Newton-Leipnik attractor";
    std::array<std::string, 3> formulae = {"-a*x + y + 10*y*z",
                                           "-x - 0.4*y + 5*x*z",
                                           "b*z - 5*x*y"};
    std::vector<std::string> constantsNames = {"a", "b"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.4, 0.175}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1]](const Model::Point &values) {
            return Model::Point{
                    -a * values.x + values.y * (1 + 10 * values.z),
                    values.x * (5 * values.z - 1) - 0.4 * values.y,
                    b * values.z - 5 * values.x * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemNoseHoover() {
    std::string attractorName = "The Nose-Hoover attractor";
    std::array<std::string, 3> formulae = {"y",
                                           "y*z - x",
                                           "a - y^2"};
    std::vector<std::string> constantsNames = {"a"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {1.5}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0]](const Model::Point &values) {
            return Model::Point{
                    values.y,
                    values.y * values.z - values.x,
                    a - values.y * values.y
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemQiChen() {
    std::string attractorName = "The Qi-Chen attractor";
    std::array<std::string, 3> formulae = {"a*(y - x) + x*z",
                                           "x*(c - z) + y",
                                           "x*y - b*z"};
    std::vector<std::string> constantsNames = {"a", "b", "c"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {38, 8.0 / 3, 80}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    a * (values.y - values.x) + values.y * values.z,
                    values.x * (c - values.z) + values.y,
                    values.x * values.y - b * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemRayleighBenard() {
    std::string attractorName = "The Rayleigh-Benard attractor";
    std::array<std::string, 3> formulae = {"a*(y - x)",
                                           "b*x - y - x*z",
                                           "x*y - c*z"};
    std::vector<std::string> constantsNames = {"a", "b", "c"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {9, 12, 5}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2]](const Model::Point &values) {
            return Model::Point{
                    a * (values.y - values.x),
                    values.x * (b - values.z) - values.y,
                    values.x * values.y - c * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemRucklige() {
    std::string attractorName = "The Rucklige attractor";
    std::array<std::string, 3> formulae = {"-a*x + b*y - y*z",
                                           "x",
                                           "y^2 - z"};
    std::vector<std::string> constantsNames = {"a", "b"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {2, 6.7}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1]](const Model::Point &values) {
            return Model::Point{
                    values.y * (b - values.z) - a * values.x,
                    values.x,
                    values.y * values.y - values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemSakaraya() {
    std::string attractorName = "The Sakaraya attractor";
    std::array<std::string, 3> formulae = {"y - x + y*z",
                                           "-x - y + a*x*z",
                                           "z - b*x*y"};
    std::vector<std::string> constantsNames = {"a", "b"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.4, 0.3}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1]](const Model::Point &values) {
            return Model::Point{
                    values.y * (values.z + 1) - values.x,
                    values.x * (a * values.z - 1) - values.y,
                    values.z - b * values.x * values.y
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemShimizuMorioka() {
    std::string attractorName = "The Shimizu-Morioka attractor";
    std::array<std::string, 3> formulae = {"y",
                                           "x*(1 - z) - a*y",
                                           "x^2 - b*z"};
    std::vector<std::string> constantsNames = {"a", "b"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.75, 0.45}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1]](const Model::Point &values) {
            return Model::Point{
                    values.y,
                    values.x * (1 - values.z) - a * values.y,
                    values.x * values.x - b * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemThomas() {
    std::string attractorName = "The Thomas attractor";
    std::array<std::string, 3> formulae = {"a*x + sin(y)",
                                           "-a*y + sin(z)",
                                           "-a*z + sin(x)"};
    std::vector<std::string> constantsNames = {"a"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.19}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0]](const Model::Point &values) {
            return Model::Point{
                    a * values.x + std::sin(values.y),
                    -a * values.y + std::sin(values.z),
                    -a * values.z + std::sin(values.x),
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemTSUCS1() {
    std::string attractorName = "The TSUCS1 attractor";
    std::array<std::string, 3> formulae = {"a*(y - z) + c*x*z",
                                           "f*y - x*z",
                                           "b*z + x*y - d*x^2"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d", "f"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {40, 0.833, 0.5, 0.65, 20}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2],
                d = constValues[3], e = constValues[4]](const Model::Point &values) {
            return Model::Point{
                    a * (values.y - values.x) + c * values.x * values.z,
                    e * values.y - values.x * values.z,
                    b * values.z + values.x * (values.y - d * values.x)
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemTSUCS2() {
    std::string attractorName = "The TSUCS2 attractor";
    std::array<std::string, 3> formulae = {"a*(y - z) + d*x*z",
                                           "g*y + b*x - x*z",
                                           "c*z + x*y - f*x^2"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d", "f", "g"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {40, 55, 1.833, 0.16, 0.65, 20}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2], d = constValues[3],
                e = constValues[4], f = constValues[5]](const Model::Point &values) {
            return Model::Point{
                    a * (values.y - values.x) + d * values.x * values.z,
                    f * values.y + values.x * (b - values.z),
                    c * values.z + values.x * (values.y - e * values.x)
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemWangSun() {
    std::string attractorName = "The Wang-Sun attractor";
    std::array<std::string, 3> formulae = {"a*x + c*y*z",
                                           "b*x + d*y - x*z",
                                           "f*z + g*x*y"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d", "f", "g"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {0.2, -0.01, 1, -0.4, -1, -1}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2], d = constValues[3],
                e = constValues[4], f = constValues[5]](const Model::Point &values) {
            return Model::Point{
                    a * values.x + c * values.y * values.z,
                    values.x * (b - values.z) + d * values.y,
                    e * values.z + f * values.x * values.y
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemWimolBanlue() {
    std::string attractorName = "The Wimol-Banlue attractor";
    std::array<std::string, 3> formulae = {"y - x",
                                           "-z*tanh(x)",
                                           "x*y + |y| - a"};
    std::vector<std::string> constantsNames = {"a"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {2}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0]](const Model::Point &values) {
            return Model::Point{
                    values.y - values.x,
                    -values.z * std::tanh(values.x),
                    values.x * values.y + std::abs(values.y) - a
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


template<typename LambdaNewPointAction>
DynamicSystem<LambdaNewPointAction> getSystemYuWang() {
    std::string attractorName = "The Yu-Wang attractor";
    std::array<std::string, 3> formulae = {"a*(y - x)",
                                           "b*x - c*x*z",
                                           "e^(x*y) - d*z"};
    std::vector<std::string> constantsNames = {"a", "b", "c", "d"};
    std::vector<std::pair<std::string, std::vector<long double>>> interestingConstants = {
            {"Classic values", {10, 40, 2, 2.5}}
    };
    auto derivativesFunctionGetter = [](std::vector<long double> constValues) {
        return [a = constValues[0], b = constValues[1], c = constValues[2],
                d = constValues[3]](const Model::Point &values) {
            return Model::Point{
                    a * (values.y - values.x),
                    values.x * (b - c * values.z),
                    std::exp(values.x * values.y) - d * values.z
            };
        };
    };
    return {attractorName, formulae, constantsNames, interestingConstants,
            DynamicSystemInternal<LambdaNewPointAction, decltype(derivativesFunctionGetter(std::declval<std::vector<long double>>()))>
                    {derivativesFunctionGetter}};
}


}} // namespace DynamicSystem::AllSystems
