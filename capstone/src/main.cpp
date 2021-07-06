
#include <iostream>

#include "opendrive_parser.hpp"
#include "tsim_simulator.hpp"

int main(int argc, char* argv) {
    parser::OpenDriveParser parser;
    auto map = parser.parse("../xodr/Town01.xodr");
    // auto map = parser.parse("../xodr/TownBig.xodr");

    tsim::Simulator sim(map);
    std::size_t num_vehicles = 1;

    for (std::size_t i = 0; i < num_vehicles; i++) sim.addVehicle();

    sim.run();
}