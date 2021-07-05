#ifndef __TSIM_MAP_HPP__
#define __TSIM_MAP_HPP__

#include <bits/stdint-uintn.h>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "tsim_util.hpp"

namespace tsim {
enum class DrivingDirection { normal, inversed };
enum class LaneGroup { left, right, center };
enum class LaneType { sidewalk, shoulder, driving, none };

class JunctionConnection {
   public:
    const uint32_t incomingRoad() const { return incoming_road_; }
    const uint32_t connectingRoad() const { return connecting_road_; }
    const DrivingDirection drivingDirection() const { return driving_direction_; }

   private:
    uint16_t id_{0};
    uint32_t incoming_road_{0};
    uint32_t connecting_road_{0};
    DrivingDirection driving_direction_{DrivingDirection::normal};

    friend class MapBuilder;
};

class Map;
class Road;

class Lane {
   public:
    // Lane(Road* road) : road_(road){};
    const Point getStart() { return lane_points_.front(); }
    // Road* getRoad() { return road_; }
    // void setOffset(double offset) { offset_ = offset; }
    // void setWidth(double width) { width_ = width; }
    // void setId(int id) { id_ = id; }
    const double width() { return width_; }
    const double id() { return id_; }
    // void setType(LaneType type) { type_ = type; }
    // void addPoints(std::vector<Point> points) { lane_points_.insert(lane_points_.end(), points.begin(),
    // points.end()); }
    const std::vector<Point>& points() const { return lane_points_; }

   private:
    std::vector<Point> lane_points_;

    int32_t id_{0};
    double offset_{0.0f};
    double width_{0.0f};
    LaneType type_;

    friend class MapBuilder;
};

class LaneSection {
   public:
    std::vector<std::shared_ptr<Lane>> lanes(LaneGroup group) const {
        switch (group) {
            case LaneGroup::left:
                return left_;
                break;
            case LaneGroup::center:
                return center_;
                break;
            case LaneGroup::right:
                return right_;
                break;
        }
    };  // TODO not const

   private:
    double s_offset_{0.0f};
    std::vector<std::shared_ptr<Lane>> left_;
    std::vector<std::shared_ptr<Lane>> center_;
    std::vector<std::shared_ptr<Lane>> right_;

    friend class MapBuilder;
};

class Junction {
   public:
    Junction(std::shared_ptr<Map> map) : map_(map){};
    const uint32_t id() const { return id_; };
    std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> findConnectingRoads(uint32_t incomingRoad);

   private:
    std::shared_ptr<Map> map_;

    uint32_t id_{0};
    std::vector<JunctionConnection> connections_;

    friend class MapBuilder;
};

class Road {
   public:
    Road(std::shared_ptr<Map> map) : map_(map){};
    std::pair<std::shared_ptr<Road>, DrivingDirection> successor();
    std::pair<std::shared_ptr<Road>, DrivingDirection> predecessor();
    const uint32_t id() const { return id_; };
    const std::vector<std::shared_ptr<LaneSection>>& sections() const { return sections_; };  // Todo not const
    const std::vector<Point>& points() const { return points_; };
    const Point startPoint() const { return points_.front(); };
    std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> findConnectingRoads(DrivingDirection direction);

   private:
    std::shared_ptr<Map> map_;

    std::vector<std::shared_ptr<LaneSection>> sections_;
    std::vector<Point> points_;

    double length_{0};
    uint16_t id_{0};
    uint32_t junction_{0};

    uint32_t predecessor_{0};
    uint32_t successor_{0};
    DrivingDirection successor_driving_direction_{DrivingDirection::normal};
    DrivingDirection predecessor_driving_direction_{DrivingDirection::normal};

    friend class MapBuilder;
};

class Map {
   public:
    Map() = default;
    Map(const Map& other) = delete;
    Map(Map&& other) = delete;
    Map operator=(const Map& other) = delete;
    Map operator=(Map&& other) = delete;

   public:
    std::shared_ptr<Road> getRandomRoad() { return roads_.at(std::rand() % roads_.size()); }
    std::vector<std::shared_ptr<Road>> roads() const { return roads_; }
    std::shared_ptr<Road> findRoadById(int id) {
        auto it =
            std::find_if(roads_.begin(), roads_.end(), [id](std::shared_ptr<Road> road) { return road->id() == id; });
        if (it != roads_.end()) return (*it);
        return nullptr;
    }
    std::shared_ptr<Junction> findJunctionById(int id) {
        auto it = std::find_if(junctions_.begin(), junctions_.end(), [id](std::shared_ptr<Junction> junction) {
            return junction->id() == id;
        });
        if (it != junctions_.end()) return (*it);
        return nullptr;
    }

   private:
    std::vector<std::shared_ptr<Road>> roads_;
    // should I use std::vector<Road> here?  or std::vector<std::unique_ptr<Road>>?
    std::vector<std::shared_ptr<Junction>> junctions_;

    friend class MapBuilder;
};

}  // namespace tsim

#endif  // __TSIM_MAP_HPP__
