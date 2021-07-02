#ifndef __TSIM_SIMULATOR_HPP__
#define __TSIM_SIMULATOR_HPP__

#include <thread>
#include <vector>

#include "tsim_graphics.hpp"
// #include "graphics_raylib.hpp"
#include "tsim_map.hpp"
#include "tsim_object.hpp"

namespace tsim {
class Simulator {
   public:
    Simulator(Map map) : map_(map), renderer_(this){};
    void run() {
        for (auto const& obj : objects_) obj->simulate();
        renderer_.display();
    };
    void addVehicle() { objects_.emplace_back(std::make_shared<Vehicle>(&map_)); };
    std::vector<std::shared_ptr<TrafficObject>> objects() { return objects_; };
    Map* map() { return &map_; };

   private:
    Map map_;
    MapsRenderer renderer_;

    // add renderer mutex
    std::vector<std::shared_ptr<TrafficObject>> objects_;
    std::vector<std::thread> threads_;
};
}  // namespace tsim
#endif  // __TSIM_SIMULATOR_HPP__
