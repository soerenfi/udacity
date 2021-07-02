#include <bits/stdint-uintn.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "tsim_map.hpp"
namespace tsim {

std::vector<std::pair<Road*, DrivingDirection>> Junction::connectingRoads(uint32_t incomingRoad) {
    std::vector<JunctionConnection> connectingRoads;
    std::copy_if(
        connections_.begin(), connections_.end(), std::back_inserter(connectingRoads),
        [incomingRoad](const JunctionConnection& connection) { return connection.incomingRoad() == incomingRoad; });

    std::vector<std::pair<Road*, DrivingDirection>> ret;
    std::transform(
        connectingRoads.cbegin(), connectingRoads.cend(), std::back_inserter(ret), [&](const JunctionConnection& elem) {
            return std::make_pair(map_->findRoadById(elem.connectingRoad()), elem.drivingDirection());
        });
    return ret;
}

std::pair<Road*, DrivingDirection> Road::successor() {
    return std::make_pair(map_->findRoadById(this->successor_), successor_driving_direction_);
}
std::pair<Road*, DrivingDirection> Road::predecessor() {
    return std::make_pair(map_->findRoadById(this->predecessor_), predecessor_driving_direction_);
}

std::vector<std::pair<Road*, DrivingDirection>> Road::findConnectingRoads(DrivingDirection direction) {
    std::vector<std::pair<Road*, DrivingDirection>> connectingRoads;  // change to shared_ptr
    // check if successor is Road
    std::pair<Road*, DrivingDirection> connectingRoad;
    switch (direction) {
        case DrivingDirection::normal:
            connectingRoad = this->successor();
            break;
        case DrivingDirection::inversed:
            connectingRoad = this->predecessor();
            break;
    }
    if (connectingRoad.first) {
        connectingRoads.push_back(connectingRoad);
    } else {
        // if not, check for junction with id
        Junction* junction{nullptr};
        switch (direction) {
            case DrivingDirection::normal:
                junction = map_->findJunctionById(successor_);
                break;
            case DrivingDirection::inversed:
                junction = map_->findJunctionById(predecessor_);
                break;
        }
        if (junction) connectingRoads = junction->connectingRoads(id());
    }
    return connectingRoads;
}

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

}  // namespace tsim