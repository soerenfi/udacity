#ifndef __OPENDRIVE_PARSER_HPP__
#define __OPENDRIVE_PARSER_HPP__

#include <tinyxml2.h>

#include "tsim_map.hpp"

class OpenDriveParser {
   public:
    OpenDriveParser(std::string filename);
    tsim::Map getMap() { return map_; }

   private:
    void parseHeader();
    void parseRoads();
    void parseJunctions();
    void calculateRoadPoints(const tinyxml2::XMLElement* odr_road, tsim::Road& road);
    tsim::DrivingDirection contactPointToDrivingDirection(const char* cp);

   private:
    tsim::Map map_;

    tinyxml2::XMLDocument xml_doc_;
};

#endif  // __OPENDRIVE_PARSER_HPP__