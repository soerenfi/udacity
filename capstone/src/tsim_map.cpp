#include <bits/stdint-uintn.h>

#include <chrono>
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

}  // namespace tsim