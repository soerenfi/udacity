#ifndef __TSIM_SIMULATOR_HPP__
#define __TSIM_SIMULATOR_HPP__

#include <thread>
#include <vector>

// #include "renderer.hpp"
#include "tsim_graphics.hpp"
#include "tsim_map.hpp"
#include "tsim_object.hpp"

namespace tsim {

class Simulator {
   public:
    Simulator(std::shared_ptr<Map> map) : map_(map), renderer_(this){};
    void run() {
        for (auto const& obj : objects_) obj->simulate();
        renderer_.display();
    };
    void addVehicle() { objects_.emplace_back(std::make_shared<Vehicle>(map_)); };
    std::vector<std::shared_ptr<TrafficObject>> objects() { return objects_; };
    std::shared_ptr<Map> map() { return map_; };

   private:
    std::shared_ptr<Map> map_;
    MapsRenderer renderer_;
    // Renderer renderer_;

    // add renderer mutex
    std::vector<std::shared_ptr<TrafficObject>> objects_;
    std::vector<std::thread> threads_;
};
}  // namespace tsim
#endif  // __TSIM_SIMULATOR_HPP__
