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
    auto step = std::chrono::milliseconds(10);
    int road_step = 0;
    int steps_taken{0};
    while (true) {
        auto now = std::chrono::system_clock::now();
        auto target = now + step;
        position_ = current_road_->points().at(road_step);

        if (steps_taken < road_size) {
            switch (current_driving_direction_) {
                case DrivingDirection::normal:
                    road_step += 1;
                    break;
                case DrivingDirection::inversed:
                    road_step -= 1;
                    break;
            }
            steps_taken++;
        } else {
            // end of road reached
            std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> roads =
                current_road_->findConnectingRoads(current_driving_direction_);
            // pick random road to continue driving
            auto randIt = roads.begin();
            std::advance(randIt, std::rand() % roads.size());

            current_road_ = (*randIt).first;
            current_driving_direction_ = (*randIt).second;
            road_size = current_road_->points().size() - 1;

            // jump to start/end of new road depending on driving direction
            switch (current_driving_direction_) {
                case DrivingDirection::normal:
                    road_step = 0;
                    break;
                case DrivingDirection::inversed:
                    road_step = current_road_->points().size() - 1;
                    break;
            }
            steps_taken = 0;
        }
        std::this_thread::sleep_until(target);
    }
};  // namespace tsim
}  // namespace tsim