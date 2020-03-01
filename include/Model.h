#pragma once

namespace Model {

struct Point {
    long double x, y, z;
};

std::vector<Point> generate_points(
        Point start_point,
        int points_count,
        int steps_per_point,
        long double tau,
        const std::vector<long double> &constants);

}//namespace Model