#ifndef __TSIM_MAP_BUILDER_HPP__
#define __TSIM_MAP_BUILDER_HPP__

#include <memory>

#include "tsim_map.hpp"

namespace tsim {

class MapBuilder {  // TODO switch to builder pattern
   public:
    MapBuilder() { map_ = std::make_shared<Map>(); }
    std::shared_ptr<Road> addRoad(int id) {
        std::shared_ptr<tsim::Road> road = std::make_shared<tsim::Road>(map_);
        road->id_ = id;
        map_->roads_.push_back(road);
        return road;
    }
    std::shared_ptr<Road> getRoad(int id) {
        auto it = std::find_if(
            map_->roads_.begin(), map_->roads_.end(), [id](std::shared_ptr<Road> road) { return road->id() == id; });
        if (it != map_->roads_.end()) return (*it);
        return nullptr;
    }
    // void addJunction(Junction&& junction) { map_.junctions_.emplace_back(std::move(junction)); }

    void road_setLength(Road* road, double length) { road->length_ = length; }
    // void setType(RoadType type) { type_ = type; };
    void road_setPredecessor(Road* road, int id, DrivingDirection direction) {
        road->predecessor_ = id;
        road->predecessor_driving_direction_ = direction;
    };
    void road_setSuccessor(Road* road, uint32_t id, DrivingDirection direction) {
        road->successor_ = id;
        road->successor_driving_direction_ = direction;
    };
    void road_addRoadPoints(Road* road, std::vector<Point> points) {
        road->points_.insert(road->points_.end(), points.begin(), points.end());
    }
    std::shared_ptr<LaneSection> road_addLaneSection(Road* road, double s_offset) {
        std::shared_ptr<LaneSection> lane_section = std::make_shared<LaneSection>();
        lane_section->s_offset_ = s_offset;
        road->sections_.push_back(lane_section);
        return lane_section;
    }
    std::shared_ptr<Lane> laneSection_addLane(
        LaneSection* lane_section, LaneGroup lane_grp, uint32_t id, double offset, double width, LaneType type) {
        std::shared_ptr<Lane> lane = std::make_shared<Lane>();
        lane->id_ = id;
        lane->offset_ = offset;
        lane->width_ = width;
        lane->type_ = type;
        switch (lane_grp) {
            case LaneGroup::left:
                lane_section->left_.push_back(lane);
                break;
            case LaneGroup::center:
                lane_section->center_.push_back(lane);
                break;
            case LaneGroup::right:
                lane_section->right_.push_back(lane);
        }
        return lane;
    }
    void lane_addLanePoints(Lane* lane, std::vector<Point> points) {
        lane->lane_points_.insert(lane->lane_points_.end(), points.begin(), points.end());
    }

    std::shared_ptr<Junction> addJunction(int id) {
        std::shared_ptr<Junction> junction = std::make_shared<Junction>(map_);
        junction->id_ = id;
        map_->junctions_.push_back(junction);
        return junction;
    }
    void junction_addConnection(
        Junction* junction, uint32_t incoming_road, uint32_t connecting_road, DrivingDirection driving_direction) {
        JunctionConnection connection;
        connection.incoming_road_ = incoming_road;
        connection.connecting_road_ = connecting_road;
        connection.driving_direction_ = driving_direction;
        junction->connections_.push_back(connection);
    }
    std::shared_ptr<Map> getMap() { return std::move(map_); }

   private:
    std::shared_ptr<Map> map_;
};
}  // namespace tsim
#endif  // __TSIM_MAP_BUILDER_HPP__