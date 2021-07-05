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
    io2d::brush m_BuildingFillBrush{io2d::rgba_color{208, 197, 190}};
    io2d::brush m_BuildingOutlineBrush{io2d::rgba_color{181, 167, 154}};
    io2d::stroke_props m_BuildingOutlineStrokeProps{1.f};
    io2d::brush m_LeisureFillBrush{io2d::rgba_color{189, 252, 193}};
    io2d::brush m_LeisureOutlineBrush{io2d::rgba_color{160, 248, 162}};
    io2d::stroke_props m_LeisureOutlineStrokeProps{1.f};
    io2d::brush m_WaterFillBrush{io2d::rgba_color{155, 201, 215}};
    io2d::brush m_RailwayStrokeBrush{io2d::rgba_color{93, 93, 93}};
    io2d::brush m_RailwayDashBrush{io2d::rgba_color::white};
    io2d::dashes m_RailwayDashes{0.f, {3.f, 3.f}};
    float m_RailwayOuterWidth = 3.f;
    float m_RailwayInnerWidth = 2.f;
};

#endif  // __TSIM_GRAPHICS_HPP__
