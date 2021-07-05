#ifndef __TSIM_MAP_HPP__
#define __TSIM_MAP_HPP__

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

#include "tsim_util.hpp"

namespace tsim {
class Road;
class Map;
enum class DrivingDirection { normal, inversed };
enum class LaneSide { left, right };
enum class LaneType { sidewalk, shoulder, driving, none };

class JunctionConnection {
   public:
    JunctionConnection(uint32_t id, uint32_t incomingRoad, uint32_t connectingRoad, DrivingDirection direction)
        : id_(id), incomingRoad_(incomingRoad), connectingRoad_(connectingRoad), drivingDirection_(direction) {}

    const uint32_t incomingRoad() const { return incomingRoad_; }
    const uint32_t connectingRoad() const { return connectingRoad_; }
    DrivingDirection drivingDirection() const { return drivingDirection_; }

   private:
    uint16_t id_{0};
    uint32_t incomingRoad_{0};
    uint32_t connectingRoad_{0};
    DrivingDirection drivingDirection_{DrivingDirection::normal};
};

class Junction {
   public:
    Junction(Map* map) : map_(map){};
    void setId(uint32_t id) { id_ = id; }
    const uint32_t id() const { return id_; };
    void addConnection(uint32_t id, uint32_t incomingRoad, uint32_t connectingRoad, DrivingDirection direction) {
        connections_.emplace_back(JunctionConnection(id, incomingRoad, connectingRoad, direction));
    }
    std::vector<std::pair<Road*, DrivingDirection>> connectingRoads(uint32_t incomingRoad);

   private:
    Map* map_;
    uint32_t id_{0};
    std::vector<JunctionConnection> connections_;
};

enum class RoadType { road, junction };

class Lane {
   public:
    // Lane(Road* road) : road_(road){};
    Point getStart() { return lanePoints_.front(); }
    // Road* getRoad() { return road_; }
    double getWidth() { return width_; }
    void setOffset(double offset) { offset_ = offset; }
    void setWidth(double width) { width_ = width; }
    void setId(int id) { id_ = id; }
    double width() { return width_; }
    double id() { return id_; }
    void setType(LaneType type) { type_ = type; }
    void addPoints(std::vector<Point> points) { lanePoints_.insert(lanePoints_.end(), points.begin(), points.end()); }
    const std::vector<Point>& points() const { return lanePoints_; }

   private:
    std::vector<Point> lanePoints_;

    // Road* road_;

    int32_t id_{0};
    LaneSide side_;
    double offset_{0.0f};
    double width_{0.0f};
    LaneType type_;
};

class LaneSection {
   public:
    void addLane(Lane&& lane) { lanes_.emplace_back(std::move(lane)); }
    const std::vector<Lane>& lanes() const { return lanes_; };  // TODO not const

   private:
    std::vector<Lane> lanes_;
    // std::vector<Lane> left_;
    // std::vector<Lane> center_;
    // std::vector<Lane> right_;
};
class Map;
class Road {
   public:
    Road(Map* map) : map_(map){};
    void setId(uint32_t id) { id_ = id; }
    void setLength(double length) { length_ = length; }
    // void setType(RoadType type) { type_ = type; };
    void setPredecessor(uint32_t id, DrivingDirection direction) {
        predecessor_ = id;
        predecessor_driving_direction_ = direction;
    };
    void setSuccessor(uint32_t id, DrivingDirection direction) {
        successor_ = id;
        successor_driving_direction_ = direction;
    };
    std::pair<Road*, DrivingDirection> successor();
    std::pair<Road*, DrivingDirection> predecessor();
    const uint32_t id() const { return id_; };
    const std::vector<LaneSection>& sections() const { return sections_; };  // Todo not const
    void addLaneSection(LaneSection&& section) { sections_.emplace_back(std::move(section)); }
    // RoadType type() const { return type_; };
    const std::vector<Point>& points() const { return points_; };
    void setPoints(std::vector<Point> points) { points_ = points; };
    Point startPoint() const { return points_.front(); };
    void setJunction(uint32_t junction) { junction_ = junction; }
    std::vector<std::pair<Road*, DrivingDirection>> findConnectingRoads(DrivingDirection direction);
    void addPoints(std::vector<Point> points) { points_.insert(points_.end(), points.begin(), points.end()); }

   private:
    Map* map_{nullptr};

    std::vector<LaneSection> sections_;
    std::vector<Point> points_;
    // RoadType type_{RoadType::road};

    double length_{0};
    uint16_t id_{0};
    uint32_t junction_{0};

    uint32_t predecessor_{0};
    uint32_t successor_{0};
    DrivingDirection successor_driving_direction_{DrivingDirection::normal};
    DrivingDirection predecessor_driving_direction_{DrivingDirection::normal};
};

class Map {
   public:
    void addRoad(Road&& road) { roads_.emplace_back(std::move(road)); }
    void addJunction(Junction&& junction) { junctions_.emplace_back(std::move(junction)); }
    void setRevMajor(uint8_t rev) { revMajor_ = rev; }
    void setRevMinor(uint8_t rev) { revMinor_ = rev; }
    Road* getRandomRoad() { return &roads_.at(std::rand() % roads_.size()); }
    const std::vector<Road>& roads() const { return roads_; }
    Road* findRoadById(int id) {
        auto it = std::find_if(roads_.begin(), roads_.end(), [id](const Road& road) { return road.id() == id; });
        if (it != roads_.end()) return &(*it);
        return nullptr;
    }
    Junction* findJunctionById(int id) {
        auto it = std::find_if(
            junctions_.begin(), junctions_.end(), [id](const Junction& junction) { return junction.id() == id; });
        if (it != junctions_.end()) return &(*it);
        return nullptr;
    }

   private:
    uint8_t revMajor_{0};
    uint8_t revMinor_{0};

    std::vector<Road> roads_;
    std::vector<Junction> junctions_;

    friend class MapBuilder;
};

class MapBuilder {  // TODO switch to builder pattern
   public:
    void addRoad(Road&& road) { map_.roads_.emplace_back(std::move(road)); }
    void addJunction(Junction&& junction) { map_.junctions_.emplace_back(std::move(junction)); }
    void setRevMajor(uint8_t rev) { map_.revMajor_ = rev; }
    void setRevMinor(uint8_t rev) { map_.revMinor_ = rev; }

    Map map_;
};
}  // namespace tsim

#endif  // __TSIM_MAP_HPP__
