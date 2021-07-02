#include <iostream>
#include <stdexcept>

#include "opendrive_parser.hpp"
#include "tsim_map.hpp"

OpenDriveParser::OpenDriveParser(std::string filename) {
    tinyxml2::XMLError eResult = xml_doc_.LoadFile(filename.c_str());
    if (eResult != tinyxml2::XML_SUCCESS) {
        if (eResult == tinyxml2::XML_ERROR_FILE_NOT_FOUND) {
            throw std::runtime_error("File not found!");
        } else {
            throw std::runtime_error("File could not be loaded!");
        }
    }
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");
    if (odr == nullptr) throw std::runtime_error("OpenDRIVE element not found");

    parseHeader();
    parseRoads();
    parseJunctions();
}
void OpenDriveParser::calculateRoadPoints(const tinyxml2::XMLElement* odr_road, tsim::Road& road) {
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
void OpenDriveParser::parseHeader() {
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");
    map_.setRevMajor(odr->FirstChildElement("header")->UnsignedAttribute("revMajor"));
    map_.setRevMinor(odr->FirstChildElement("header")->UnsignedAttribute("revMinor"));
}
void OpenDriveParser::parseRoads() {
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");
    for (auto* odrRoad = odr->FirstChildElement("road"); odrRoad != nullptr;
         odrRoad = odrRoad->NextSiblingElement("road")) {
        std::cout << "road found " << odrRoad->UnsignedAttribute("id") << std::endl;
        tsim::Road road(&map_);
        auto predecessor = odrRoad->FirstChildElement("link")->FirstChildElement("predecessor");
        if (predecessor) {
            if (!strcmp(predecessor->Attribute("elementType"), "road")) {
                road.setPredecessor(
                    predecessor->UnsignedAttribute("elementId"),
                    contactPointToDrivingDirection(predecessor->Attribute("contactPoint")));
            }
            if (!strcmp(predecessor->Attribute("elementType"), "junction")) {
                road.setPredecessor(predecessor->UnsignedAttribute("elementId"), tsim::DrivingDirection::normal);
            };
        }
        auto successor = odrRoad->FirstChildElement("link")->FirstChildElement("successor");
        if (successor) {
            // if not junction
            if (!strcmp(successor->Attribute("elementType"), "road")) {
                road.setSuccessor(
                    successor->UnsignedAttribute("elementId"),
                    contactPointToDrivingDirection(successor->Attribute("contactPoint")));
            }
            if (!strcmp(successor->Attribute("elementType"), "junction")) {
                road.setSuccessor(successor->UnsignedAttribute("elementId"), tsim::DrivingDirection::normal);
            }
        }
        road.setId(odrRoad->UnsignedAttribute("id"));

        for (auto* lane = odrRoad->FirstChildElement("lane"); lane != nullptr;
             lane = lane->NextSiblingElement("lane")) {
            // calculateLanePoints(road, lane);
        }
        calculateRoadPoints(odrRoad, road);
        map_.addRoad(std::move(road));
    }
}
tsim::DrivingDirection OpenDriveParser::contactPointToDrivingDirection(const char* cp) {
    std::cout << "drdir " << cp << std::endl;
    if (!strcmp(cp, "start")) return tsim::DrivingDirection::normal;
    if (!strcmp(cp, "end")) return tsim::DrivingDirection::inversed;
    throw std::runtime_error("unknown contact point");
}
void OpenDriveParser::parseJunctions() {
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");
    for (auto* odrJunction = odr->FirstChildElement("junction"); odrJunction != nullptr;
         odrJunction = odrJunction->NextSiblingElement("junction")) {
        tsim::Junction junction(&map_);
        junction.setId(odrJunction->UnsignedAttribute("id"));
        for (auto* odrConnection = odrJunction->FirstChildElement("connection"); odrConnection != nullptr;
             odrConnection = odrConnection->NextSiblingElement("connection")) {
            junction.addConnection(
                odrConnection->UnsignedAttribute("id"), odrConnection->UnsignedAttribute("incomingRoad"),
                odrConnection->UnsignedAttribute("connectingRoad"),
                contactPointToDrivingDirection(odrConnection->Attribute("contactPoint")));
        }
        map_.addJunction(std::move(junction));
    }
}
