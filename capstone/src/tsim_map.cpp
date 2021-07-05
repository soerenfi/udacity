#include <bits/stdint-uintn.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "tsim_map.hpp"
namespace tsim {

std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> Junction::findConnectingRoads(uint32_t incomingRoad) {
    std::vector<JunctionConnection> connectingRoads;
    std::copy_if(
        connections_.begin(), connections_.end(), std::back_inserter(connectingRoads),
        [incomingRoad](const JunctionConnection& connection) { return connection.incomingRoad() == incomingRoad; });

    std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> ret;
    std::transform(
        connectingRoads.cbegin(), connectingRoads.cend(), std::back_inserter(ret), [&](const JunctionConnection& elem) {
            return std::make_pair(map_->findRoadById(elem.connectingRoad()), elem.drivingDirection());
        });
    return ret;
}

std::pair<std::shared_ptr<Road>, DrivingDirection> Road::successor() {
    return std::make_pair(map_->findRoadById(this->successor_), successor_driving_direction_);
}
std::pair<std::shared_ptr<Road>, DrivingDirection> Road::predecessor() {
    return std::make_pair(map_->findRoadById(this->predecessor_), predecessor_driving_direction_);
}

std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> Road::findConnectingRoads(DrivingDirection direction) {
    std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> connecting_roads;  // change to shared_ptr
    // check if successor is Road
    std::pair<std::shared_ptr<Road>, DrivingDirection> connecting_road;
    switch (direction) {
        case DrivingDirection::normal:
            connecting_road = this->successor();
            break;
        case DrivingDirection::inversed:
            connecting_road = this->predecessor();
            break;
    }
    if (connecting_road.first) {
        connecting_roads.push_back(connecting_road);
    } else {
        // if not, check for junction with id
        std::shared_ptr<Junction> junction;
        switch (direction) {
            case DrivingDirection::normal:
                junction = map_->findJunctionById(successor_);
                break;
            case DrivingDirection::inversed:
                junction = map_->findJunctionById(predecessor_);
                break;
        }
        if (junction) connecting_roads = junction->findConnectingRoads(id());
    }
    return connecting_roads;
}

}  // namespace tsim