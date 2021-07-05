
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
    // parser::OpenDriveParser parser("../xodr/TownBig.xodr");
    parser::OpenDriveParser parser;
    auto map = parser.parse("../xodr/Town01.xodr");

    tsim::Simulator sim(map);

    sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();
    // sim.addVehicle();

    sim.run();
}