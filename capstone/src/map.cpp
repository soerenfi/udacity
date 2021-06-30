#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "map.hpp"
namespace sim {
void Road::calculateRoadPoints(double x, double y, double hdg, double length) {
    points_.emplace_back(Point(x, y));
    auto end = Point(x + length * std::cos(hdg), y + length * std::sin(hdg));
    double road_length{0};
    double step = 1.f;
    while (road_length < length) {
        Point next((x += step * std::cos(hdg)), (y += step * std::sin(hdg)));
        road_length += step;
        points_.emplace_back(next);
    }
    points_.emplace_back(end);
}
template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
void Road::calculateRoadPoints(double x, double y, double hdg, double length, double arc) {
    points_.emplace_back(Point(x, y));
    // points_.emplace_back(Point(0, 0));
    auto x_m = x + 1 / arc * -std::sin(hdg);
    auto y_m = y + 1 / arc * std::cos(hdg);
    // points_.emplace_back(Point(x_m, y_m));
    double road_length{0};
    double step = 0.1f;
    double start_angle = hdg - sgn(arc) * M_PI / 2;
    double end_angle = start_angle + length * arc;

    for (double angle = start_angle; sgn(arc) * angle < sgn(arc) * end_angle; angle += length * arc / 20) {
        Point next((x_m + 1 / std::abs(arc) * std::cos(angle)), (y_m + 1 / std::abs(arc) * std::sin(angle)));
        points_.emplace_back(next);
    }
}

}  // namespace sim