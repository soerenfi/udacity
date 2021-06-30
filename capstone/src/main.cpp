#include <tinyxml2.h>

#include <iostream>

#include "graphics.hpp"
#include "graphics_raylib.hpp"
#include "map.hpp"

class OdrParser {
   public:
    void parseHeader(const tinyxml2::XMLElement* odr);
    std::vector<sim::Road> parseRoads(const tinyxml2::XMLElement* odr);
    void calculateRoadPoints(const tinyxml2::XMLElement* odr_road, sim::Road& road);

   private:
    sim::Map map_;
};

void OdrParser::calculateRoadPoints(const tinyxml2::XMLElement* odr_road, sim::Road& road) {
    auto plan_view = odr_road->FirstChildElement("planView");
    for (auto* geom = plan_view->FirstChildElement("geometry"); geom != nullptr;
         geom = geom->NextSiblingElement("geometry")) {
        auto s = geom->DoubleAttribute("s");
        auto x = geom->DoubleAttribute("x");
        auto y = geom->DoubleAttribute("y");
        auto hdg = geom->DoubleAttribute("hdg");
        auto length = geom->DoubleAttribute("length");

        // line, spiral, arc, poly3, parampoly3
        auto arc = geom->FirstChildElement("arc");
        if (arc != nullptr) {
            std::cout << "arc detected" << std::endl;
            road.calculateRoadPoints(x, y, hdg, length, arc->DoubleAttribute("curvature"));

        } else
            road.calculateRoadPoints(x, y, hdg, length);
    }
}

void OdrParser::parseHeader(const tinyxml2::XMLElement* odr) {
    map_.setRevMajor(odr->FirstChildElement("header")->UnsignedAttribute("revMajor"));
    map_.setRevMinor(odr->FirstChildElement("header")->UnsignedAttribute("revMinor"));
}
std::vector<sim::Road> OdrParser::parseRoads(const tinyxml2::XMLElement* odr) {
    for (auto* odrRoad = odr->FirstChildElement("road"); odrRoad != nullptr;
         odrRoad = odrRoad->NextSiblingElement("road")) {
        std::cout << "road found " << std::endl;
        sim::Road road;
        auto predecessor = odrRoad->FirstChildElement("link")->FirstChildElement("predecessor");
        auto successor = odrRoad->FirstChildElement("link")->FirstChildElement("successor");
        road.setPredecessor(predecessor->UnsignedAttribute("id"));
        road.setSuccessor(successor->UnsignedAttribute("id"));
        road.setId(odrRoad->UnsignedAttribute("id"));
        for (auto* lane = odrRoad->FirstChildElement("lane"); lane != nullptr;
             lane = lane->NextSiblingElement("lane")) {
            // calculateLanePoints(road, lane);
        }
        calculateRoadPoints(odrRoad, road);
        map_.addRoad(std::move(road));
    }
    return map_.getRoads();
}

int main() {
    tinyxml2::XMLDocument xml_doc;
    std::string filename("../xodr/Town01.xodr");
    // std::string filename("../xodr/TownBig.xodr");
    tinyxml2::XMLError eResult = xml_doc.LoadFile(filename.c_str());
    if (eResult != tinyxml2::XML_SUCCESS) {
        if (eResult == tinyxml2::XML_ERROR_FILE_NOT_FOUND) {
            throw std::runtime_error("File not found!");
        } else {
            throw std::runtime_error("File could not be loaded!");
        }
    }
    auto odr = xml_doc.FirstChildElement("OpenDRIVE");
    if (odr == nullptr) throw std::runtime_error("OpenDRIVE element bot found");

    OdrParser parser;
    parser.parseHeader(odr);
    auto roads = parser.parseRoads(odr);

    // RaylibGraphics renderer(roads);
    // renderer.render();

    MapsRenderer renderer(roads);
    renderer.display();
}