#ifndef __OPENDRIVE_PARSER_HPP__
#define __OPENDRIVE_PARSER_HPP__

#include <tinyxml2.h>

#include "tsim_map.hpp"
#include "tsim_util.hpp"

namespace parser {
class OpenDriveParser {
   public:
    OpenDriveParser(std::string filename);
    tsim::Map getMap() { return map_; }

   private:
    void parseHeader();
    void parseRoads();
    void parseLanes();
    void parseJunctions();
    void parseLanes(tsim::LaneSection& laneSection, const tinyxml2::XMLElement* section);
    void parseLane(tsim::Lane& lane, const tinyxml2::XMLElement* odrLane);
    void calculateRoadPoints(const tinyxml2::XMLElement* odr_road, tsim::Road& road);
    void calculateLanePoints(tsim::Lane& lane, const tinyxml2::XMLElement* odrLane);
    std::vector<tsim::Point> calculateStraight(double x, double y, double hdg, double length, double offset = 0);
    std::vector<tsim::Point> calculateArc(double x, double y, double hdg, double length, double arc, double offset = 0);

    tsim::DrivingDirection contactPointToDrivingDirection(const char* cp);
    tsim::LaneType parseLaneType(const char* lt);

   private:
    tsim::Map map_;

    tinyxml2::XMLDocument xml_doc_;
};
}  // namespace parser

#endif  // __OPENDRIVE_PARSER_HPP__