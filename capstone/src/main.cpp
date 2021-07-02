
#include <iostream>

#include "opendrive_parser.hpp"
#include "tsim_simulator.hpp"

int main() {
    // tinyxml2::XMLDocument xml_doc;
    // std::string filename("../xodr/Town01.xodr");
    // // std::string filename("../xodr/TownBig.xodr");
    // tinyxml2::XMLError eResult = xml_doc.LoadFile(filename.c_str());
    // if (eResult != tinyxml2::XML_SUCCESS) {
    //     if (eResult == tinyxml2::XML_ERROR_FILE_NOT_FOUND) {
    //         throw std::runtime_error("File not found!");
    //     } else {
    //         throw std::runtime_error("File could not be loaded!");
    //     }
    // }
    // auto odr = xml_doc.FirstChildElement("OpenDRIVE");
    // if (odr == nullptr) throw std::runtime_error("OpenDRIVE element bot found");

    OpenDriveParser parser("../xodr/Town01.xodr");
    // OpenDriveParser parser("../xodr/TownBig.xodr");
    auto map = parser.getMap();

    tsim::Simulator sim(map);

    sim.addVehicle();
    // sim.addVehicle();

    sim.run();

    // RaylibGraphics renderer(roads);
    // renderer.render();
}