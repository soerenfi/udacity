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
enum class RoadType { road, junction };

class Map;
class Road;
class LaneSection;
class Lane {
   public:
    Lane(std::shared_ptr<LaneSection> lane_section) : lane_section_(lane_section){};
    const Point startPoint() { return lane_points_.front(); }
    const double width() { return width_; }
    const double id() { return id_; }
    int nextLane(DrivingDirection direction);  // more lane sections in current road?

    const std::vector<Point>& points() const { return lane_points_; }
    const std::vector<Point>& boundaryPoints() const { return lane_boundary_points_; }
    std::vector<std::shared_ptr<Lane>> predecessors() { return predecessors_; }
    std::vector<std::shared_ptr<Lane>> successors() { return successors_; }

   private:
    std::shared_ptr<LaneSection> lane_section_;
    std::vector<Point> lane_points_;
    std::vector<Point> lane_boundary_points_;

    std::vector<std::shared_ptr<Lane>> successors_;
    std::vector<std::shared_ptr<Lane>> predecessors_;

    int32_t predecessor_{0};
    int32_t successor_{0};

    int32_t id_{0};
    double offset_{0.0f};
    double width_{0.0f};
    LaneType type_;

    friend class MapBuilder;
};

class LaneSection {
   public:
    LaneSection(std::shared_ptr<Road> road) : road_(road){};
    std::vector<std::shared_ptr<Lane>> lanes() const { return lanes_; }
    std::shared_ptr<Lane> lane(int id) const;
    std::shared_ptr<Road> road() { return road_; };
    std::vector<std::shared_ptr<LaneSection>> predecessors() { return predecessors_; }
    std::vector<std::shared_ptr<LaneSection>> successors() { return successors_; }

   private:
    std::shared_ptr<Road> road_;
    std::vector<std::shared_ptr<Lane>> lanes_;

    std::vector<std::shared_ptr<LaneSection>> successors_;
    std::vector<std::shared_ptr<LaneSection>> predecessors_;

    double s_offset_{0.0f};

    friend class MapBuilder;
};
struct LaneLink {
    int from_{0};
    int to_{0};
};
class JunctionConnection {
   public:
    const uint32_t incomingRoad() const { return incoming_road_; }
    const uint32_t connectingRoad() const { return connecting_road_; }
    // const DrivingDirection drivingDirection() const { return driving_direction_; }
    const std::vector<LaneLink>& laneLinks() const { return lane_links_; }

   private:
    uint16_t id_{0};
    uint32_t incoming_road_{0};
    uint32_t connecting_road_{0};
    DrivingDirection driving_direction_{DrivingDirection::normal};
    std::vector<LaneLink> lane_links_;

    friend class MapBuilder;
};
class Junction {
   public:
    Junction(std::shared_ptr<Map> map) : map_(map){};
    const uint32_t id() const { return id_; };
    std::vector<std::shared_ptr<JunctionConnection>> connections() { return connections_; };
    // std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> findConnectingRoads(uint32_t incomingRoad);
    std::vector<std::pair<std::shared_ptr<Lane>, DrivingDirection>> findConnectingLanes(
        int current_road, int current_lane);

   private:
    std::shared_ptr<Map> map_;

    uint32_t id_{0};
    std::vector<std::shared_ptr<JunctionConnection>> connections_;

    friend class MapBuilder;
};

class Road {
   public:
    Road(std::shared_ptr<Map> map) : map_(map){};

    const uint32_t id() const { return id_; };
    const Point startPoint() const { return points_.front(); };
    std::vector<std::shared_ptr<Road>> successors() { return successors_; };
    std::vector<std::shared_ptr<Road>> predecessors() { return predecessors_; };
    int junction() { return junction_; };
    std::vector<std::pair<std::shared_ptr<Road>, DrivingDirection>> findConnectingRoads(DrivingDirection direction);
    std::shared_ptr<Lane> getFirstLane() {
        auto lane_section = sections_.front();  // TODO multiple lane sections
        auto lanes = lane_section->lanes();
        return lanes.front();
    };
    std::shared_ptr<Lane> getLaneById(int id) {
        auto lane_section = sections_.front();  // TODO multiple lane sections
        auto lanes = lane_section->lanes();
        auto it =
            std::find_if(lanes.begin(), lanes.end(), [id](std::shared_ptr<Lane> lane) { return lane->id() == id; });
        return *it;
    }
    std::vector<std::shared_ptr<LaneSection>> sections() const { return sections_; };  // Todo not const
    const std::vector<Point>& points() const { return points_; };

   private:
    std::shared_ptr<Map> map_;

    std::vector<std::shared_ptr<LaneSection>> sections_;
    std::vector<Point> points_;

    double length_{0};
    uint16_t id_{0};
    int junction_{0};
    RoadType road_type_;

    std::vector<std::shared_ptr<Road>> successors_;
    std::vector<std::shared_ptr<Road>> predecessors_;

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
