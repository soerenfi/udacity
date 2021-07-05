#include <cmath>
#include <iostream>
#include <stdexcept>

#include "opendrive_parser.hpp"
#include "tsim_map.hpp"

using namespace tsim;
namespace parser {

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
void OpenDriveParser::calculateRoadPoints(const tinyxml2::XMLElement* odr_road, Road& road) {
    auto plan_view = odr_road->FirstChildElement("planView");
    for (auto* geom = plan_view->FirstChildElement("geometry"); geom != nullptr;
         geom = geom->NextSiblingElement("geometry")) {
        std::cout << odr_road->Attribute("name") << std::endl;
        auto s = geom->DoubleAttribute("s");
        auto x = geom->DoubleAttribute("x");
        auto y = geom->DoubleAttribute("y");
        auto hdg = geom->DoubleAttribute("hdg");
        auto length = geom->DoubleAttribute("length");

        // line, spiral, arc, poly3, parampoly3
        auto arc = geom->FirstChildElement("arc");

        std::vector<Point> points;
        if (arc != nullptr)
            points = calculateArc(x, y, hdg, length, arc->DoubleAttribute("curvature"));
        else
            points = calculateStraight(x, y, hdg, length);
        road.addPoints(points);
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
        Road road(&map_);
        auto predecessor = odrRoad->FirstChildElement("link")->FirstChildElement("predecessor");
        if (predecessor) {
            if (!strcmp(predecessor->Attribute("elementType"), "road")) {
                road.setPredecessor(
                    predecessor->UnsignedAttribute("elementId"),
                    contactPointToDrivingDirection(predecessor->Attribute("contactPoint")));
            }
            if (!strcmp(predecessor->Attribute("elementType"), "junction")) {
                road.setPredecessor(predecessor->UnsignedAttribute("elementId"), DrivingDirection::normal);
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
                road.setSuccessor(successor->UnsignedAttribute("elementId"), DrivingDirection::normal);
            }
        }
        road.setId(odrRoad->UnsignedAttribute("id"));

        auto* odrLanes = odrRoad->FirstChildElement("lanes");
        for (auto* odrLaneSection = odrLanes->FirstChildElement("laneSection"); odrLaneSection != nullptr;
             odrLaneSection = odrLaneSection->NextSiblingElement("laneSection")) {
            LaneSection laneSection;

            auto* left = odrLaneSection->FirstChildElement("left");
            auto* center = odrLaneSection->FirstChildElement("center");
            auto* right = odrLaneSection->FirstChildElement("right");

            if (left) parseLanes(laneSection, left);
            // if (center) parseLanes(laneSection, center);
            if (right) parseLanes(laneSection, right);
            road.addLaneSection(std::move(laneSection));
        }
        calculateRoadPoints(odrRoad, road);
        map_.addRoad(std::move(road));
    }
}

void OpenDriveParser::parseJunctions() {
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");
    for (auto* odrJunction = odr->FirstChildElement("junction"); odrJunction != nullptr;
         odrJunction = odrJunction->NextSiblingElement("junction")) {
        Junction junction(&map_);
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
void OpenDriveParser::parseLanes(LaneSection& laneSection, const tinyxml2::XMLElement* section) {
    for (auto* odrLane = section->FirstChildElement("lane"); odrLane != nullptr;
         odrLane = odrLane->NextSiblingElement("lane")) {
        if (!strcmp(odrLane->Attribute("type"), "driving")) {
            std::cout << "found lane" << std::endl;
            Lane lane;
            parseLane(lane, odrLane);
            calculateLanePoints(lane, odrLane);
            laneSection.addLane(std::move(lane));
        }
    }
}

void OpenDriveParser::parseLane(Lane& lane, const tinyxml2::XMLElement* odrLane) {
    lane.setOffset(odrLane->FirstChildElement("width")->DoubleAttribute("sOffset"));
    std::cout << odrLane->Attribute("id") << std::endl;
    lane.setId(odrLane->IntAttribute("id"));                                   // TODO wrong; int converison not working
    lane.setWidth(odrLane->FirstChildElement("width")->DoubleAttribute("a"));  // TODO add polynomial for offset
    lane.setType(parseLaneType(odrLane->Attribute("type")));
}

void OpenDriveParser::calculateLanePoints(Lane& lane, const tinyxml2::XMLElement* odrLane) {
    auto* odrRoad = odrLane->Parent()->Parent()->Parent()->Parent();
    auto plan_view = odrRoad->FirstChildElement("planView");
    auto offset = lane.width() * sgn(lane.id());
    for (auto* geom = plan_view->FirstChildElement("geometry"); geom != nullptr;
         geom = geom->NextSiblingElement("geometry")) {
        auto s = geom->DoubleAttribute("s");
        auto x = geom->DoubleAttribute("x");
        auto y = geom->DoubleAttribute("y");
        auto hdg = geom->DoubleAttribute("hdg");
        auto length = geom->DoubleAttribute("length");

        // line, spiral, arc, poly3, parampoly3
        auto arc = geom->FirstChildElement("arc");

        std::vector<Point> points;
        if (arc != nullptr)
            points = calculateArc(x, y, hdg, length, arc->DoubleAttribute("curvature"), offset);

        else
            points = calculateStraight(x, y, hdg, length, offset);
        lane.addPoints(points);
    }
}

std::vector<Point> OpenDriveParser::calculateStraight(double x, double y, double hdg, double length, double offset) {
    std::vector<Point> points;

    double x_offset = -offset * std::sin(hdg);
    double y_offset = offset * std::cos(hdg);

    points.emplace_back(Point(x + x_offset, y + y_offset));
    auto end = Point(x + length * std::cos(hdg) + x_offset, y + length * std::sin(hdg) + y_offset);

    double road_length{0};
    double step = 1.f;
    while (road_length < length) {
        Point next(((x += step * std::cos(hdg)) + x_offset), ((y += step * std::sin(hdg)) + y_offset));
        road_length += step;
        points.emplace_back(next);
    }
    points.emplace_back(end);
    return points;
}

std::vector<Point> OpenDriveParser::calculateArc(
    double x, double y, double hdg, double length, double arc, double offset) {
    std::vector<Point> points;
    double x_offset = -offset * std::sin(hdg);
    double y_offset = offset * std::cos(hdg);
    points.emplace_back(Point(x + x_offset, y + y_offset));

    auto x_m = x + 1 / arc * -std::sin(hdg);
    auto y_m = y + 1 / arc * std::cos(hdg);
    double road_length{0};
    double step = 0.1f;
    double start_angle = hdg - sgn(arc) * M_PI / 2;
    double end_angle = start_angle + length * arc;

    Point end(
        (x_m + (1 / std::abs(arc) + sgn(arc) * -offset) * std::cos(end_angle)),
        (y_m + (1 / std::abs(arc) + sgn(arc) * -offset) * std::sin(end_angle)));

    for (double angle = start_angle; sgn(arc) * angle < sgn(arc) * end_angle; angle += length * arc / 20) {
        Point next(
            (x_m + (1 / std::abs(arc) + sgn(arc) * -offset) * std::cos(angle)),
            (y_m + (1 / std::abs(arc) + sgn(arc) * -offset) * std::sin(angle)));
        points.emplace_back(next);
    }
    // TODO add proper end point
    points.emplace_back(end);

    return points;
}

DrivingDirection OpenDriveParser::contactPointToDrivingDirection(const char* cp) {
    if (!strcmp(cp, "start")) return DrivingDirection::normal;
    if (!strcmp(cp, "end")) return DrivingDirection::inversed;
    throw std::runtime_error("unknown contact point");
}
LaneType OpenDriveParser::parseLaneType(const char* lt) {
    if (!strcmp(lt, "sidewalk")) return LaneType::sidewalk;
    if (!strcmp(lt, "shoulder")) return LaneType::shoulder;
    if (!strcmp(lt, "driving")) return LaneType::driving;
    if (!strcmp(lt, "none")) return LaneType::none;
    return LaneType::none;
}
}  // namespace parser