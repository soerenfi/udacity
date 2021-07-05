#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "SDL.h"
#include "tsim_graphics.hpp"
#include "tsim_map.hpp"

constexpr std::size_t kScreenWidth{1920};
constexpr std::size_t kScreenHeight{1080};

class Renderer {
   public:
    Renderer(tsim::Simulator* sim);
    ~Renderer();

    void render();
    void findMaxMinValues();

   private:
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    tsim::Simulator* simulator_;
    std::shared_ptr<tsim::Map> map_;
    std::vector<std::shared_ptr<tsim::TrafficObject>> objects_;

    float min_x{0};
    float max_x{0};
    float min_y{0};
    float max_y{0};

    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    std::size_t target_frame_duration{1000 / 60};
    int close{0};

    const std::size_t screen_width{kScreenWidth};
    const std::size_t screen_height{kScreenHeight};
};

#endif