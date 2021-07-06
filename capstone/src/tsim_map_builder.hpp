#ifndef __TSIM_MAP_BUILDER_HPP__
#define __TSIM_MAP_BUILDER_HPP__

#include <memory>

#include "tsim_map.hpp"

namespace tsim {

class MapBuilder {
   public:
    MapBuilder() { map_ = std::make_shared<Map>(); }
    std::shared_ptr<Road> addRoad(int id, int junction) {
        std::shared_ptr<tsim::Road> road = std::make_shared<tsim::Road>(map_);
        road->id_ = id;
        road->junction_ = junction;
        if (junction != -1)
            road->road_type_ = RoadType::junction;
        else
            road->road_type_ = RoadType::road;
        map_->roads_.push_back(road);
        return road;
    }
    std::shared_ptr<Road> getRoad(int id) {
        auto it = std::find_if(
            map_->roads_.begin(), map_->roads_.end(), [id](std::shared_ptr<Road> road) { return road->id() == id; });
        if (it != map_->roads_.end()) return (*it);
        return nullptr;
    }

    std::shared_ptr<Junction> getJunction(int id) {
        auto it = std::find_if(
            map_->junctions_.begin(), map_->junctions_.end(),
            [id](std::shared_ptr<Junction> junction) { return junction->id() == id; });
        if (it != map_->junctions_.end()) return (*it);
        return nullptr;
    }

    std::vector<std::shared_ptr<Road>> junction_findConnectingRoads(Junction* junction, Road* road) {
        std::vector<std::shared_ptr<JunctionConnection>> connecting_roads;
        std::copy_if(
            junction->connections_.begin(), junction->connections_.end(), std::back_inserter(connecting_roads),
            [road](std::shared_ptr<JunctionConnection> connection) {
                return connection->incomingRoad() == road->id();
            });

        std::vector<std::shared_ptr<Road>> ret;
        std::transform(
            connecting_roads.cbegin(), connecting_roads.cend(), std::back_inserter(ret),
            [&](std::shared_ptr<JunctionConnection> elem) { return map_->findRoadById(elem->connectingRoad()); });
        return ret;
    }

    void road_setLength(Road* road, double length) { road->length_ = length; }
    // void setType(RoadType type) { type_ = type; };

    void road_addPredecessor(Road* road, std::shared_ptr<Road> predecessor) {
        road->predecessors_.push_back(predecessor);
    };
    void road_addSuccessor(Road* road, std::shared_ptr<Road> successor) { road->successors_.push_back(successor); };
    void road_addRoadPoints(Road* road, std::vector<Point> points) {
        road->points_.insert(road->points_.end(), points.begin(), points.end());
    }
    std::shared_ptr<LaneSection> road_addLaneSection(std::shared_ptr<Road> road, double s_offset) {
        std::shared_ptr<LaneSection> lane_section = std::make_shared<LaneSection>(road);
        lane_section->s_offset_ = s_offset;
        road->sections_.push_back(lane_section);
        return lane_section;
    }
    std::shared_ptr<Lane> laneSection_addLane(
        std::shared_ptr<LaneSection> lane_section, uint32_t id, double offset, double width, LaneType type) {
        std::shared_ptr<Lane> lane = std::make_shared<Lane>(lane_section);
        lane->id_ = id;
        lane->offset_ = offset;
        lane->width_ = width;
        lane->type_ = type;
        lane_section->lanes_.push_back(lane);
        return lane;
    }
    void laneSection_addPredecessor(LaneSection* lane_section, std::shared_ptr<LaneSection> predecessor) {
        lane_section->predecessors_.push_back(predecessor);
    }
    void laneSection_addSuccessor(LaneSection* lane_section, std::shared_ptr<LaneSection> successor) {
        lane_section->successors_.push_back(successor);
    }
    void lane_addLanePoints(Lane* lane, std::vector<Point> points) {
        lane->lane_points_.insert(lane->lane_points_.end(), points.begin(), points.end());
    }
    void lane_addLaneBoundaryPoints(Lane* lane, std::vector<Point> points) {
        lane->lane_boundary_points_.insert(lane->lane_boundary_points_.end(), points.begin(), points.end());
    }
    void lane_addPredecessor(Lane* lane, std::shared_ptr<Lane> predecessor) {
        lane->predecessors_.push_back(predecessor);
    }
    void lane_addSuccessor(Lane* lane, std::shared_ptr<Lane> successor) { lane->successors_.push_back(successor); }

    std::shared_ptr<Junction> addJunction(int id) {
        std::shared_ptr<Junction> junction = std::make_shared<Junction>(map_);
        junction->id_ = id;
        map_->junctions_.push_back(junction);
        return junction;
    }
    std::shared_ptr<JunctionConnection> junction_addConnection(
        Junction* junction, uint32_t incoming_road, uint32_t connecting_road, DrivingDirection driving_direction) {
        std::shared_ptr<JunctionConnection> connection = std::make_shared<JunctionConnection>();
        connection->incoming_road_ = incoming_road;
        connection->connecting_road_ = connecting_road;
        connection->driving_direction_ = driving_direction;
        junction->connections_.push_back(connection);
        return connection;
    }

    void connection_addLaneLink(JunctionConnection* connection, int from, int to) {
        LaneLink lane_link;
        lane_link.from_ = from;
        lane_link.to_ = to;
        connection->lane_links_.push_back(lane_link);
    }

    std::shared_ptr<Map> getMap() { return std::move(map_); }

   private:
    std::shared_ptr<Map> map_;
};
}  // namespace tsim
#endif  // __TSIM_MAP_BUILDER_HPP__