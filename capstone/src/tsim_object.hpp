#ifndef __TSIM_OBJECT_HPP__
#define __TSIM_OBJECT_HPP__

#include <algorithm>
#include <cstdint>
#include <thread>
#include <vector>

#include "tsim_geometric.hpp"
#include "tsim_map.hpp"

namespace tsim {

class TrafficObject {
   public:
    TrafficObject(Map* map) {
        map_ = map;
        currentRoad_ = map_->getRandomRoad();
        // currentLane_ = currentRoad_->getFirstLane();
        position_ = currentRoad_->startPoint();
        id_ = idCounter_++;
    };
    virtual ~TrafficObject() {
        std::for_each(threads_.begin(), threads_.end(), [](std::thread& t) { t.join(); });
    }

    virtual void simulate(){};
    const Point& position() const { return position_; };

   protected:
    Point position_{0.0f, 0.0f};

    std::vector<std::thread> threads_;
    uint16_t id_{0};
    Map* map_;
    Road* currentRoad_;
    Lane* currentLane_;

    static uint16_t idCounter_;
};

class Vehicle : public TrafficObject {
   public:
    Vehicle(Map* map) : TrafficObject(map){};
    virtual void simulate() override;

   private:
    void drive();

   private:
    DrivingDirection currentDrivingDirection_{DrivingDirection::normal};
};

}  // namespace tsim

#endif  // __TSIM_OBJECT_HPP__
