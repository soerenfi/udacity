#include <iostream>
#include <thread>

#include "tsim_map.hpp"
#include "tsim_object.hpp"

namespace tsim {
uint16_t TrafficObject::id_counter_{0};

void Vehicle::simulate() {
    std::cout << "starting simulation" << std::endl;
    threads_.emplace_back(std::thread(&Vehicle::drive, this));
};
void Vehicle::drive() {
    auto road_size = current_road_->points().size() - 1;
    auto lane_size = current_lane_->points().size() - 1;
    auto step = std::chrono::milliseconds(10);
    int road_step = 0;
    int lane_step = 0;
    int steps_taken{0};
    while (true) {
        auto now = std::chrono::system_clock::now();
        auto target = now + step;
        // position_ = current_road_->points().at(road_step);
        position_ = current_lane_->points().at(lane_step);

        if (steps_taken < lane_size) {
            if (current_lane_id_ < 0) {
                road_step += 1;
                lane_step += 1;
            } else {
                road_step -= 1;
                lane_step -= 1;
            }
            steps_taken++;
        }

        else {
            // end of road reached
            std::vector<std::shared_ptr<Road>> next_roads;
            std::vector<std::shared_ptr<Lane>> next_lanes;
            if (current_lane_id_ < 0) {
                next_roads = current_road_->successors();
                next_lanes = current_lane_->successors();
            } else {
                next_roads = current_road_->predecessors();
                next_lanes = current_lane_->predecessors();
            }
            // pick random road to continue driving
            auto rand_it = next_lanes.begin();
            std::advance(rand_it, std::rand() % next_lanes.size());
            // current_road_ = (*rand_it);
            current_lane_ = (*rand_it);

            road_size = current_road_->points().size() - 1;
            lane_size = current_lane_->points().size() - 1;
            current_lane_id_ = current_lane_->id();

            //     // get following lane
            //     // if (current_road_->junction() == -1)
            //     // current_lane_ = current_road_->getLaneById(
            //     // current_lane_->nextLane(current_driving_direction_));  // continue on same lane id
            //     // else
            //     // current_lane_ = current_road_->getFirstLane();
            //     // lane_size = current_lane_->points().size() - 1;

            // jump to start/end of new road depending on driving direction
            if (current_lane_id_ < 0) {
                road_step = 0;
                lane_step = 0;

            } else {
                road_step = current_road_->points().size() - 1;
                lane_step = current_lane_->points().size() - 1;
            }
            steps_taken = 0;
        }
        std::this_thread::sleep_until(target);
    }
}
}  // namespace tsim