#include <bits/stdint-uintn.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "tsim_map.hpp"
namespace tsim {

std::shared_ptr<Lane> LaneSection::lane(int id) const {
    auto it = std::find_if(lanes_.begin(), lanes_.end(), [id](std::shared_ptr<Lane> lane) { return lane->id() == id; });
    return *it;
}

// std::vector<std::pair<std::shared_ptr<LaneSection>, DrivingDirection>> LaneSection::nextLaneSections(
//     int current_lane, DrivingDirection direction) {
//     // more lane sections in current road?
//     // auto lane_section_successor = lane_section_->successor();
//     // road_->nextLaneSection();
//     auto road = road_->findConnectingRoads(id_, direction);
// };

// int Lane::nextLane(DrivingDirection direction) {  // more lane sections in current road?
//     // auto lane_section_successor = lane_section_->successor();
//     // return lane_section_successor->lane(successor_);
//     if (direction == DrivingDirection::normal) return successor_;
//     if (direction == DrivingDirection::inversed) return predecessor_;
// };

// std::vector<std::pair<std::shared_ptr<Lane>, DrivingDirection>> Lane::nextLanes(DrivingDirection direction) {
//     std::vector<std::pair<std::shared_ptr<Lane>, DrivingDirection>> ret;
//     // get next lane sections
//     std::vector<std::pair<std::shared_ptr<LaneSection>, DrivingDirection>> lane_section =
//         lane_section_->nextLaneSections(id_, direction);
//     // get possible next lanes and directions
//     for (auto& elem : lane_section) {
//         if (direction == DrivingDirection::normal)
//             ret.push_back(std::make_pair(elem.first->lane(successor_), DrivingDirection::normal));
//         else
//             ret.push_back(std::make_pair(elem.first->lane(predecessor_), DrivingDirection::normal));
//     }
//     return ret;
// }

// std::vector<std::pair<std::shared_ptr<Lane>, DrivingDirection>> Junction::findConnectingLanes(
//     int current_road, int current_lane, DrivingDirection driving_direction) {
//     std::vector<std::shared_ptr<JunctionConnection>> connecting_roads;
//     std::copy_if(
//         connections_.begin(), connections_.end(), std::back_inserter(connecting_roads),
//         [current_road](std::shared_ptr<JunctionConnection> connection) {
//             return connection->incomingRoad() == current_road;
//         });

//     std::vector<std::pair<std::shared_ptr<Lane>, DrivingDirection>> ret;
//     std::transform(
//         connecting_roads.cbegin(), connecting_roads.cend(), std::back_inserter(ret),
//         [&](std::shared_ptr<JunctionConnection> elem) {
//             //    check_for lane link;
//             auto lane_link = std::find_if(
//                 elem->laneLinks().begin(), elem->laneLinks().end(),
//                 [current_lane](const LaneLink& lane_link) { return lane_link.from_ == current_lane; });
//             int to = lane_link->to_;
//             return std::make_pair(map_->findRoadById(elem->connectingRoad())->getLaneById(to),
//             driving_direction);
//         });

//     return ret;
// }

// std::pair<std::shared_ptr<Road>, DrivingDirection> Road::successor() {
//     return std::make_pair(map_->findRoadById(this->successor_));
// }
// std::pair<std::shared_ptr<Road>, DrivingDirection> Road::predecessor() {
//     return std::make_pair(map_->findRoadById(this->predecessor_), predecessor_driving_direction_);
// }

// std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> Road::findConnectingRoads(DrivingDirection
// direction)
// {
//     std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> connecting_roads;  // change to shared_ptr
//     // check if successor is Road
//     std::pair<std::shared_ptr<Road>, DrivingDirection> connecting_road;
//     switch (direction) {
//         case DrivingDirection::normal:
//             connecting_road = this->successor();
//             break;
//         case DrivingDirection::inversed:
//             connecting_road = this->predecessor();
//             break;
//     }
//     if (connecting_road.first) {
//         connecting_roads.push_back(connecting_road);
//     } else {
//         // if not, check for junction with id
//         std::shared_ptr<Junction> junction;
//         switch (direction) {
//             case DrivingDirection::normal:
//                 junction = map_->findJunctionById(successor_);
//                 break;
//             case DrivingDirection::inversed:
//                 junction = map_->findJunctionById(predecessor_);
//                 break;
//         }
//         if (junction) connecting_roads = junction->findConnectingRoads(id_);
//     }
//     return connecting_roads;
// }

}  // namespace tsim