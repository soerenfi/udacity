#include <iostream>
#include <thread>

#include "tsim_map.hpp"
#include "tsim_object.hpp"

namespace tsim {
uint16_t TrafficObject::idCounter_{0};

void Vehicle::simulate() {
    std::cout << "starting simulation" << std::endl;
    threads_.emplace_back(std::thread(&Vehicle::drive, this));
};
void Vehicle::drive() {
    auto road_size = currentRoad_->points().size() - 1;
    auto step = std::chrono::milliseconds(10);
    int road_step = 0;
    int steps_taken{0};
    while (true) {
        auto now = std::chrono::system_clock::now();
        auto target = now + step;
        position_ = currentRoad_->points().at(road_step);

        if (steps_taken < road_size) {
            switch (currentDrivingDirection_) {
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
            std::vector<std::pair<Road*, DrivingDirection>> roads =
                currentRoad_->findConnectingRoads(currentDrivingDirection_);
            // pick random road to continue driving
            auto randIt = roads.begin();
            std::advance(randIt, std::rand() % roads.size());

            currentRoad_ = (*randIt).first;
            currentDrivingDirection_ = (*randIt).second;
            road_size = currentRoad_->points().size() - 1;

            // jump to start/end of new road depending on driving direction
            switch (currentDrivingDirection_) {
                case DrivingDirection::normal:
                    road_step = 0;
                    break;
                case DrivingDirection::inversed:
                    road_step = currentRoad_->points().size() - 1;
                    break;
            }
            steps_taken = 0;
        }
        std::this_thread::sleep_until(target);
    }
};  // namespace tsim
}  // namespace tsim