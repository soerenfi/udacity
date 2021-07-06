#ifndef __OPENDRIVE_PARSER_HPP__
#define __OPENDRIVE_PARSER_HPP__

#include <tinyxml2.h>

#include "tsim_map_builder.hpp"
#include "tsim_util.hpp"

namespace parser {
class OpenDriveParser {
   public:
    std::shared_ptr<tsim::Map> parse(std::string filename);

   private:
    void parseHeader();
    void parseRoads();
    void parseLanes();
    void parseJunctions();
    void roadConnections();
    void parseLaneSections(std::shared_ptr<tsim::LaneSection> lane_section, const tinyxml2::XMLElement* group);
    void parseLane(tsim::Lane* lane, const tinyxml2::XMLElement* odrLane);
    void laneSectionConnections(std::shared_ptr<tsim::LaneSection> lane_section, const tinyxml2::XMLElement* group);
    void calculateRoadPoints(tsim::Road* road, const tinyxml2::XMLElement* odr_road);
    void calculateLanePoints(tsim::Lane* lane, const tinyxml2::XMLElement* odrLane);
    void calculateLaneBoundaryPoints(tsim::Lane* lane, const tinyxml2::XMLElement* odrLane);
    std::vector<tsim::Point> calculateStraight(double x, double y, double hdg, double length, double offset = 0);
    std::vector<tsim::Point> calculateArc(double x, double y, double hdg, double length, double arc, double offset = 0);

    tsim::DrivingDirection contactPointToDrivingDirection(const char* cp);
    tsim::LaneType parseLaneType(const char* lt);

   private:
    tsim::MapBuilder map_builder_;
    tinyxml2::XMLDocument xml_doc_;
};
}  // namespace parser

#endif  // __OPENDRIVE_PARSER_HPP__