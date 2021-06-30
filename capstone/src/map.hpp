#ifndef __MAP_H__
#define __MAP_H__

#include <cstdint>
#include <string>
#include <vector>

#include "geometric.hpp"

namespace sim {

class LaneSection {};

enum LaneType : uint8_t { SHOULDER, DRIVING, NONE };
enum RoadType : uint8_t { ROAD, JUNCTION };

class Lane {
   public:
    void calculateLanePoints(double x, double y, double hdg, double length);

   private:
    int id_{0};
    LaneType type_;
    std::vector<LaneSection> sections_;
    std::vector<Point> lanePoints_;
};

class Road {
   public:
    void setId(int id) { id_ = id; }
    void setLength(double length) { length_ = length; }
    void setType(RoadType type) { type_ = type; };
    void setPredecessor(int id) { predecessor_ = id; };
    void setSuccessor(int id) { successor_ = id; };
    RoadType getType() { return type_; };
    void setJunction(int junction) { junction_ = junction; }
    void calculateRoadPoints(double x, double y, double hdg, double length);
    void calculateRoadPoints(double x, double y, double hdg, double length, double arc);

    std::vector<Point> points_;

   private:
    RoadType type_;
    int predecessor_{0};
    int successor_{0};
    std::vector<Lane> lanes_;
    std::string name_;
    double length_{0};
    int id_{0};
    int junction_{0};
};

class Map {
   public:
    void addRoad(sim::Road&& road) { roads_.emplace_back(std::move(road)); }
    void setRevMajor(int rev) { revMajor_ = rev; }
    void setRevMinor(int rev) { revMinor_ = rev; }
    Road getRandomRoad() { return roads_.at(std::rand() % roads_.size()); }
    std::vector<Road> getRoads() { return roads_; }

   private:
    int revMajor_{0};
    int revMinor_{0};

    std::vector<Road> roads_;
};
}  // namespace sim

#endif  // __MAP_H__