#ifndef __TSIM_OBJECT_HPP__
#define __TSIM_OBJECT_HPP__

#include <algorithm>
#include <cstdint>
#include <thread>
#include <vector>

#include "tsim_map.hpp"
#include "tsim_util.hpp"

namespace tsim {

class TrafficObject {
   public:
    TrafficObject(std::shared_ptr<Map> map) : map_(map) {
        current_road_ = map_->getRandomRoad();
        // currentLane_ = currentRoad_->getFirstLane();
        position_ = current_road_->startPoint();
        id_ = id_counter_++;
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
    std::shared_ptr<Map> map_;
    std::shared_ptr<Road> current_road_;
    std::shared_ptr<Lane> current_lane_;

    static uint16_t id_counter_;
};

class Vehicle : public TrafficObject {
   public:
    Vehicle(std::shared_ptr<Map> map) : TrafficObject(map){};
    virtual void simulate() override;

   private:
    void drive();

   private:
    DrivingDirection current_driving_direction_{DrivingDirection::normal};
};

}  // namespace tsim

#endif  // __TSIM_OBJECT_HPP__
