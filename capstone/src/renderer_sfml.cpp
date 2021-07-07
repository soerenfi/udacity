#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#include <iostream>
#include <string>

#include "renderer.hpp"
#include "tsim_graphics.hpp"
#include "tsim_simulator.hpp"

Renderer::Renderer(tsim::Simulator* sim) : simulator_(sim) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create Window
    sdl_window = SDL_CreateWindow(
        "Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);

    if (nullptr == sdl_window) {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create renderer
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == sdl_renderer) {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
    findMaxMinValues();
    SDL_Point ViewPointOffset;

    SDL_RenderSetLogicalSize(sdl_renderer, (max_x - min_x) * 10, (max_y - min_y) * 10);
    // SDL_RenderSetScale(sdl_renderer, (max_x - min_x), (max_y - min_y));
    SDL_RenderSetScale(sdl_renderer, 1, 1);

    map_ = simulator_->map();
}

Renderer::~Renderer() {
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Renderer::findMaxMinValues() {
    for (const auto& road : map_->roads()) {
        for (const auto& pt : road->points()) {
            if (pt.x() > max_x) max_x = pt.x();
            if (pt.x() < min_x) min_x = pt.x();
            if (pt.y() > max_y) max_y = pt.y();
            if (pt.y() < min_y) min_y = pt.y();  // use std::sort?
        }
    }
};
void Renderer::render() {
    while (!close) {
        SDL_Event event;

        frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                close = 1;
            }
        }

        objects_ = simulator_->objects();

        // Clear screen
        SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
        SDL_RenderClear(sdl_renderer);

        // Render food
        // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);

        // block.x = 0;
        // block.y = 1;
        // SDL_RenderFillRect(sdl_renderer, &block);

        // Render snake's body
        // SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        // for (SDL_Point const& point : snake.body) {
        //     block.x = point.x * block.w;
        //     block.y = point.y * block.h;
        //     SDL_RenderFillRect(sdl_renderer, &block);

        // }

        for (const auto& road : map_->roads()) {
            // if (auto rep_it = m_RoadReps.find(line.type); rep_it != m_RoadReps.end()) {
            // auto& rep = rep_it->second;
            auto width = .5f;
            size_t num_points = road->points().size();
            SDL_Point points[num_points];
            for (size_t i = 0; i < road->points().size(); ++i) {
                points[i] = {road->points().at(i).x(), -road->points().at(i).y()};
            }
            SDL_RenderDrawPoints(sdl_renderer, points, num_points);
        }

        for (const auto object : objects_) {
            SDL_Point points[5];
            points[0] = {object->position().x() + 2, object->position().y() + 2};
            points[1] = {object->position().x() + 2, object->position().y() - 2};
            points[2] = {object->position().x() - 2, object->position().y() - 2};
            points[3] = {object->position().x() - 2, object->position().y() + 2};
            points[4] = {object->position().x() + 2, object->position().y() + 2};
            SDL_RenderDrawPoints(sdl_renderer, points, 5);
        }

        // // Render snake's head
        // block.x = static_cast<int>(snake.head_x) * block.w;
        // block.y = static_cast<int>(snake.head_y) * block.h;
        // if (snake.alive) {
        //     SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
        // } else {
        //     SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
        // }
        // SDL_RenderFillRect(sdl_renderer, &block);

        // Update Screen
        SDL_RenderPresent(sdl_renderer);

        // Input, Update, Render - the main game loop.
        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.

        if (frame_duration < target_frame_duration) {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}