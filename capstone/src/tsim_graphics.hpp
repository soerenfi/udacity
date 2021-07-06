#ifndef __TSIM_GRAPHICS_HPP__
#define __TSIM_GRAPHICS_HPP__

#include <io2d.h>

#include <unordered_map>

#include "tsim_object.hpp"

using namespace std::experimental;

namespace tsim {
class Simulator;
class Map;
class TrafficObject;
}  // namespace tsim

class MapsRenderer {
   public:
    MapsRenderer(tsim::Simulator* sim);
    void display();

   private:
    void render();

    //     void BuildRoadReps();
    // void BuildLanduseBrushes();

    void findMaxMinValues();

    // void drawBuildings();
    void drawRoads();
    void drawLanes();
    void drawLaneBoundaries();
    // void drawRailways();
    // void drawLeisure();
    // void drawWater();
    // void drawLanduses();
    // void drawStartPosition();
    // void drawEndPosition();
    void drawPath();
    void drawAxes();
    void drawGrid();
    // void drawLine();
    void drawVehicles();
    // io2d::interpreted_path pathLine();

   private:
    tsim::Simulator* simulator_;
    std::shared_ptr<tsim::Map> map_;
    std::vector<std::shared_ptr<tsim::TrafficObject>> objects_;

    float min_x{0};
    float max_x{0};
    float min_y{0};
    float max_y{0};

    float scale_{1000.f};
    float m_PixelsInMeter = 1.f;

    io2d::matrix_2d matrix_;

    io2d::output_surface surface_;

    io2d::brush m_BackgroundFillBrush{io2d::rgba_color{56, 60, 74}};
    io2d::brush lane_brush_{io2d::rgba_color{255, 255, 255}};
    io2d::brush road_brush_{io2d::rgba_color{255, 255, 255}};
    float m_RailwayOuterWidth = 3.f;
    float m_RailwayInnerWidth = 2.f;
};

#endif  // __TSIM_GRAPHICS_HPP__
