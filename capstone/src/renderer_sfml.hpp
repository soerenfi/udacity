#ifndef __RENDERER_SFML_HPP__
#define __RENDERER_SFML_HPP__

#include <vector>

#include <SFML/Graphics.hpp>

#include "tsim_graphics.hpp"
#include "tsim_map.hpp"

constexpr std::size_t kScreenWidth{1920};
constexpr std::size_t kScreenHeight{1080};

class Renderer {
   public:
    Renderer(tsim::Simulator* sim);
    ~Renderer(){};

    void render();

    void drawLanes();
    void drawVehicles();


    void findMaxMinValues();

   private:
    std::unique_ptr<sf::RenderWindow> window_;

    tsim::Simulator* simulator_;
    std::shared_ptr<tsim::Map> map_;
    std::vector<std::shared_ptr<tsim::TrafficObject>> objects_;

    double min_x{0};
    double max_x{0};
    double min_y{0};
    double max_y{0};

    double scale_;
    int frame_count = 0;
    std::size_t target_frame_duration{1000 / 60};
    int close{0};

    const std::size_t screen_width{kScreenWidth};
    const std::size_t screen_height{kScreenHeight};
};

#endif  // __RENDERER_SFML_HPP__