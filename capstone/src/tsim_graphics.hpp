#ifndef __TSIM_GRAPHICS_HPP__
#define __TSIM_GRAPHICS_HPP__

#include <io2d.h>

#include <memory>
#include <unordered_map>
#include <vector>

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

    void findMaxMinValues();

    void drawRoads();
    void drawLanes();
    void drawLaneBoundaries();
    void drawPath();
    void drawAxes();
    void drawVehicles();

   private:
    tsim::Simulator* simulator_;
    std::shared_ptr<tsim::Map> map_;
    std::vector<std::shared_ptr<tsim::TrafficObject>> objects_;

    float min_x{0};
    float max_x{0};
    float min_y{0};
    float max_y{0};

    float scale_{1000.f};

    io2d::matrix_2d matrix_;
    io2d::output_surface surface_;

    io2d::brush background_brush_{io2d::rgba_color{56, 60, 74}};
    io2d::brush lane_brush_{io2d::rgba_color{255, 255, 255}};
    io2d::brush road_brush_{io2d::rgba_color{255, 255, 255}};
};

#endif  // __TSIM_GRAPHICS_HPP__
