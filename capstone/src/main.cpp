
#include <iostream>

#include "opendrive_parser.hpp"
#include "tsim_simulator.hpp"

int main() {
    // parser::OpenDriveParser parser("../xodr/TownBig.xodr");
    parser::OpenDriveParser parser("../xodr/Town01.xodr");
    auto map = parser.getMap();

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