
#include <iostream>

#include "opendrive_parser.hpp"
#include "tsim_simulator.hpp"

int main(int argc, char* argv) {
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};
    parser::OpenDriveParser parser;
    auto map = parser.parse("../xodr/Town01.xodr");
    // auto map = parser.parse("../xodr/TownBig.xodr");

    tsim::Simulator sim(map);
    std::size_t num_vehicles = 4;

    for (std::size_t i = 0; i < num_vehicles; i++) sim.addVehicle();

    sim.run();
}