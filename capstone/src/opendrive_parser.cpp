#include <cmath>
#include <iostream>
#include <stdexcept>

#include "opendrive_parser.hpp"
#include "tsim_map.hpp"

using namespace tsim;
namespace parser {

std::shared_ptr<Map> OpenDriveParser::parse(std::string filename) {
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
    parseLanes();
    return map_builder_.getMap();
}
void OpenDriveParser::parseHeader() {
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");
    auto rev_major = odr->FirstChildElement("header")->UnsignedAttribute("revMajor");
    auto rev_minor = odr->FirstChildElement("header")->UnsignedAttribute("revMinor");
}

void OpenDriveParser::parseRoads() {
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");

    for (auto* odrRoad = odr->FirstChildElement("road"); odrRoad != nullptr;
         odrRoad = odrRoad->NextSiblingElement("road")) {
        auto road = map_builder_.addRoad(odrRoad->UnsignedAttribute("id"));

        auto predecessor = odrRoad->FirstChildElement("link")->FirstChildElement("predecessor");
        if (predecessor) {
            if (!strcmp(predecessor->Attribute("elementType"), "road")) {
                map_builder_.road_setPredecessor(
                    road.get(), predecessor->UnsignedAttribute("elementId"),
                    contactPointToDrivingDirection(predecessor->Attribute("contactPoint")));
            }
            if (!strcmp(predecessor->Attribute("elementType"), "junction")) {
                map_builder_.road_setPredecessor(
                    road.get(), predecessor->UnsignedAttribute("elementId"), DrivingDirection::normal);
            };
        }
        auto successor = odrRoad->FirstChildElement("link")->FirstChildElement("successor");
        if (successor) {
            // if not junction
            if (!strcmp(successor->Attribute("elementType"), "road")) {
                map_builder_.road_setSuccessor(
                    road.get(), successor->UnsignedAttribute("elementId"),
                    contactPointToDrivingDirection(successor->Attribute("contactPoint")));
            }
            if (!strcmp(successor->Attribute("elementType"), "junction")) {
                map_builder_.road_setSuccessor(
                    road.get(), successor->UnsignedAttribute("elementId"), DrivingDirection::normal);
            }
        }
        calculateRoadPoints(road.get(), odrRoad);
    }
}

void OpenDriveParser::calculateRoadPoints(Road* road, const tinyxml2::XMLElement* odr_road) {
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
        if (arc != nullptr) {
            std::cout << "arc" << std::endl;
            points = calculateArc(x, y, hdg, length, arc->DoubleAttribute("curvature"));
        } else
            points = calculateStraight(x, y, hdg, length);

        // add points for this geometry to road description
        map_builder_.road_addRoadPoints(road, points);
    }
}
void OpenDriveParser::parseJunctions() {
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");
    for (auto* odrJunction = odr->FirstChildElement("junction"); odrJunction != nullptr;
         odrJunction = odrJunction->NextSiblingElement("junction")) {
        auto junction = map_builder_.addJunction(odrJunction->UnsignedAttribute("id"));

        for (auto* odrConnection = odrJunction->FirstChildElement("connection"); odrConnection != nullptr;
             odrConnection = odrConnection->NextSiblingElement("connection")) {
            map_builder_.junction_addConnection(
                junction.get(), odrConnection->UnsignedAttribute("incomingRoad"),
                odrConnection->UnsignedAttribute("connectingRoad"),
                contactPointToDrivingDirection(odrConnection->Attribute("contactPoint")));
        }
    }
}

void OpenDriveParser::parseLanes() {
    auto odr = xml_doc_.FirstChildElement("OpenDRIVE");
    for (auto* odrRoad = odr->FirstChildElement("road"); odrRoad != nullptr;
         odrRoad = odrRoad->NextSiblingElement("road")) {
        auto* odrLanes = odrRoad->FirstChildElement("lanes");
        for (auto* odrLaneSection = odrLanes->FirstChildElement("laneSection"); odrLaneSection != nullptr;
             odrLaneSection = odrLaneSection->NextSiblingElement("laneSection")) {
            std::shared_ptr<Road> road = map_builder_.getRoad(odrRoad->UnsignedAttribute("id"));
            std::shared_ptr<LaneSection> lane_section =
                map_builder_.road_addLaneSection(road.get(), odrLaneSection->DoubleAttribute("s"));

            auto* group_left = odrLaneSection->FirstChildElement("left");
            auto* group_center = odrLaneSection->FirstChildElement("center");
            auto* group_right = odrLaneSection->FirstChildElement("right");

            if (group_left) parseLaneSections(lane_section.get(), group_left, tsim::LaneGroup::left);
            // if (group_center) parseLanes(lane_section, group_center, center);
            if (group_right) parseLaneSections(lane_section.get(), group_right, tsim::LaneGroup::right);
        }
    }
}
void OpenDriveParser::parseLaneSections(
    LaneSection* lane_section, const tinyxml2::XMLElement* group, tsim::LaneGroup lane_group) {
    for (auto* odrLane = group->FirstChildElement("lane"); odrLane != nullptr;
         odrLane = odrLane->NextSiblingElement("lane")) {
        if (!strcmp(odrLane->Attribute("type"), "driving")) {
            std::shared_ptr<Lane> lane = map_builder_.laneSection_addLane(
                lane_section, lane_group, odrLane->IntAttribute("id"),
                odrLane->FirstChildElement("width")->DoubleAttribute("sOffset"),
                odrLane->FirstChildElement("width")->DoubleAttribute("a"), parseLaneType(odrLane->Attribute("type")));
            calculateLanePoints(lane.get(), odrLane);
        }
    }
}

void OpenDriveParser::calculateLanePoints(Lane* lane, const tinyxml2::XMLElement* odrLane) {
    auto* odrRoad = odrLane->Parent()->Parent()->Parent()->Parent();

    auto plan_view = odrRoad->FirstChildElement("planView");
    auto offset = lane->width() * sgn(lane->id());
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
        map_builder_.lane_addLanePoints(lane, points);
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