#pragma once

#include <functional>

namespace Model {

enum class ModelName {
    ROSSLER,
    LORENZ,
    CHEN_LEE,
    AIZAWA,
    HENON,
    HR3,
    CHUA,
    GIN,
    ANISHCHENKO_ASTAKHOV,
    ARNEODO,
    BOUALI_2,
    BURKE_SHAW,
    CHEN_CELIKOVSKY,
    COULLET,
    DADRAS,
    DEQUAN_LI,
    FINANCE,
    FOUR_WING,
    GENESIO_TESI,
    HADLEY,
    HALVORSEN,
    LIU_CHEN,
    LORENZ_MOD_1,
    LORENZ_MOD_2,
    LU_CHEN,
    NEWTON_LEIPNIK,
    NOSE_HOOVER,
    QI_CHEN,
    RAYLEIGH_BENARD,
    RUCKLIDGE,
    SAKARAYA,
    SHIMIZU_MORIOKA,
    THOMAS,
    TSUCS1,
    TSUCS2,
    WANG_SUN,
    WIMOL_BANLUE,
    YU_WANG
};

struct Point {
    long double x, y, z;
};

void generate_points(
        const std::function<void(const Point &)> &new_point_action, //calls when generate new point
        Point start_point,
        int points_count,
        int steps_per_point,
        long double tau,
        ModelName modelName,
        const std::vector<long double> &constants);

} // namespace Model
